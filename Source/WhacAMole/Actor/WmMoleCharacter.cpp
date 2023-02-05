// Copyright Epic Games, Inc. All Rights Reserved.

#include "WmMoleCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "../DataAsset/WmGlobalsDataAsset.h"
#include "../Resource/WmGlobals.h"
#include "WmVeggieSpawner.h"
#include "WmGardenerCharacter.h"


AWmMoleCharacter::AWmMoleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	MoleRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MoleRoot"));
	MoleRoot->SetupAttachment(GetRootComponent());

	HitBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(GetRootComponent());
}

void AWmMoleCharacter::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	float AnimationRatio = bIsBurrowed ? 1.0f : 0.0f;

	// Entered burrowed state
	if (bIsBurrowed && !BurrowAnimationStartTime && !UndergroundStartTime)
	{
		UndergroundStartTime = UGameplayStatics::GetUnpausedTimeSeconds(this);
	}

	// Max underground time exceeded
	/*if (UndergroundStartTime)
	{
		const float CurrentUndergroundTime = UGameplayStatics::GetUnpausedTimeSeconds(this) - *UndergroundStartTime;
		if (CurrentUndergroundTime > MaxUndergroundDuration)
		{
			UndergroundStartTime.Reset();
			ToggleBurrowInternal();
		}
	}*/

	// Burrow state transition
	{
		if (BurrowAnimationStartTime)
		{
			const float TimeSinceAnimationStart = FMath::Max(0.0f, UGameplayStatics::GetUnpausedTimeSeconds(this) - *BurrowAnimationStartTime);
			const float Ratio = TimeSinceAnimationStart / (bIsBurrowed ? BurrowAnimationDuration : UnburrowAnimationDuration);

			AnimationRatio = FMath::Min(Ratio, 1.0f);
			if (!bIsBurrowed) { AnimationRatio = 1.0f - AnimationRatio; }

			if (Ratio > 1.0f)
			{
				BurrowAnimationStartTime.Reset();

				if (!bIsBurrowed)
				{
					if (const UWmGlobalsDataAsset* GlobalsDataAsset = UWmGlobalsDataAsset::Get(this))
					{
						UGameplayStatics::PlaySoundAtLocation(this, GlobalsDataAsset->UnburrowSound, GetActorLocation(), GetActorRotation());
					}
				}
			}
		}

		if (MoleRoot)
		{
			MoleRoot->SetRelativeLocation(FVector::DownVector * AnimationRatio * BurrowDepth);
		}
	}

	// Update stun
	if (StunStartTime)
	{
		const float TimeSinceAnimationStart = FMath::Max(0.0f, UGameplayStatics::GetUnpausedTimeSeconds(this) - *StunStartTime);
		if (TimeSinceAnimationStart > StunDuration)
		{
			SetStunnedMaterial(false);
			StunStartTime.Reset();
		}
	}

	AWmVeggieSpawner* OldClosestVeggie = ClosestVeggie;
	ClosestVeggie = CalculateClosestVeggie(FeedbackRadius);

	if (OldClosestVeggie && OldClosestVeggie != ClosestVeggie)
	{
		OldClosestVeggie->SetWobbleAmount(0.0f);
	}

	const bool bWasInsideForceFeedbackArea = OldClosestVeggie != nullptr;
	const bool bIsInsideForceFeedbackArea = ClosestVeggie != nullptr;
	const UWmGlobalsDataAsset* GlobalsDataAsset = UWmGlobalsDataAsset::Get(this);
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (GlobalsDataAsset && PlayerController)
	{
		if (bWasInsideForceFeedbackArea != bIsInsideForceFeedbackArea)
		{
			if (bIsInsideForceFeedbackArea)
			{
				FForceFeedbackParameters Params;
				Params.bLooping = true;
				PlayerController->ClientPlayForceFeedback(GlobalsDataAsset->ForceFeedbackEffect, Params);
			}
			else
			{
				PlayerController->ClientStopForceFeedback(GlobalsDataAsset->ForceFeedbackEffect, NAME_None);
			}
		}
		if (bIsInsideForceFeedbackArea)
		{
			const float feedbackScale = [&]()->float
			{
				const float distance = FVector::Dist2D(GetActorLocation(), ClosestVeggie->GetActorLocation());
				if (distance > FeedbackRadius) { return 1.0f; }
				return 0.6f * ((FeedbackRadius - PickUpRadius) - (FVector::Dist2D(GetActorLocation(), ClosestVeggie->GetActorLocation()) - PickUpRadius)) / (FeedbackRadius - PickUpRadius);
			}();
			//const float feedbackScale = (FeedbackRadius - FVector::Dist2D(GetActorLocation(), ClosestVeggie->GetActorLocation())) / FeedbackRadius;
			PlayerController->ForceFeedbackScale = feedbackScale;
			ClosestVeggie->SetWobbleAmount(feedbackScale);
		}
	}
}

bool AWmMoleCharacter::IsTargetable() const
{
	return (!bIsBurrowed || BurrowAnimationStartTime) && !IsStunned();
}

bool AWmMoleCharacter::IsStunned() const
{
	return StunStartTime.IsSet();
}

void AWmMoleCharacter::ApplyStun()
{
	StunStartTime = UGameplayStatics::GetUnpausedTimeSeconds(this);
	SetStunnedMaterial(true);
	if (bIsBurrowed) { bIsBurrowed = false; }
}

void AWmMoleCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			PlayerController->SetViewTarget(*ActorItr);
			break;
		}
		/*if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}*/
	}

	FWmGlobals* Globals = FWmGlobals::Get(this);
	if (ensure(Globals))
	{
		Globals->Mole = this;
	}
}

void AWmMoleCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Worx not!
	// Set up action bindings
	/*if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWmGardenerCharacter::Test);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWmGardenerCharacter::Move);

		//Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWhacAMoleCharacter::Look);
	}*/

	PlayerInputComponent->BindAxis("MoveForward", this, &AWmMoleCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWmMoleCharacter::MoveRight);

	PlayerInputComponent->BindAction("ToggleBurrow", IE_Released, this, &AWmMoleCharacter::ToggleBurrow);
	PlayerInputComponent->BindAction("PickUp", IE_Released, this, &AWmMoleCharacter::TryPickUp);
}

/*void AWmGardenerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWmGardenerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}*/

void AWmMoleCharacter::MoveForward(float Value)
{
	if (!bIsBurrowed || IsStunned()) { return;}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWmMoleCharacter::MoveRight(float Value)
{
	if (!bIsBurrowed || IsStunned()) { return;}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AWmMoleCharacter::ToggleBurrow()
{
	if (IsStunned()) { return; }
	ToggleBurrowInternal();
}

void AWmMoleCharacter::SetStunnedMaterial(bool bStunned)
{
	FWmGlobals* Globals = FWmGlobals::Get(this);
	if (Globals && Globals->GlobalsDataAsset.IsValid())
	{
		TArray<UActorComponent*> MeshComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
		for (UActorComponent* C : MeshComponents)
		{
			UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(C);
			if (MeshComponent)
			{
				if (bStunned)
				{
					CachedMaterials.Add(MeshComponent, MeshComponent->GetMaterial(0));
					MeshComponent->SetMaterial(0, Globals->GlobalsDataAsset->StunnedMaterial);
				}
				else
				{
					if (UMaterialInterface** Material = CachedMaterials.Find(MeshComponent))
					{
						MeshComponent->SetMaterial(0, *Material);
					}
					else
					{
						MeshComponent->EmptyOverrideMaterials();
					}
				}
			}
		}
	}
}

AWmVeggieSpawner* AWmMoleCharacter::CalculateClosestVeggie(float MaxDistance) const
{
	AWmVeggieSpawner* Result = nullptr;
	if (FWmGlobals* Globals = FWmGlobals::Get(this))
	{
		TOptional<float> minDistSquared;
		for (const TWeakObjectPtr<AWmVeggieSpawner>& Veggie : Globals->VeggieSpawners)
		{
			if (Veggie.IsValid())
			{
				const float distSquared = FVector::DistSquared2D(GetActorLocation(), Veggie->GetActorLocation());
				if (distSquared > FMath::Square(MaxDistance)) { continue; }
				if (!minDistSquared || distSquared < *minDistSquared)
				{
					minDistSquared = distSquared;
					Result = Veggie.Get();
				}
			}
		}
	}
	return Result;
}

void AWmMoleCharacter::ToggleBurrowInternal()
{
	if (!BurrowAnimationStartTime)
	{
		bIsBurrowed = !bIsBurrowed;
		if (!bIsBurrowed)
		{
			UndergroundStartTime.Reset();
			FWmGlobals* Globals = FWmGlobals::Get(this);
			AWmGardenerCharacter* Gardener = Globals ? Globals->Gardener.Get() : nullptr;
			if (Gardener)
			{
				const float distanceSq = FVector::DistSquared2D(GetActorLocation(), Gardener->GetActorLocation());
				if (distanceSq < FMath::Square(StunRadius))
				{
					Gardener->ApplyStun();
				}
			}
		}
		BurrowAnimationStartTime = UGameplayStatics::GetTimeSeconds(this);
	}
}

void AWmMoleCharacter::TryPickUp()
{
	if (bIsBurrowed || BurrowAnimationStartTime || IsStunned()) { return; }
	if (ClosestVeggie && FVector::DistSquared2D(GetActorLocation(), ClosestVeggie->GetActorLocation()) < FMath::Square(PickUpRadius))
	{
		FWmGlobals* Globals = FWmGlobals::Get(this);
		const UWmGlobalsDataAsset* GlobalsDataAsset = UWmGlobalsDataAsset::Get(this);
		if (Globals && GlobalsDataAsset)
		{
			int32 numPoints = ClosestVeggie->TryPickMole();
			if (numPoints != INDEX_NONE)
			{
				Globals->MolePoints += numPoints;
				UGameplayStatics::PlaySoundAtLocation(this, GlobalsDataAsset->PickUpVeggie, GetActorLocation(), GetActorRotation());
				UE_LOG(LogTemp, Warning, TEXT("Mole Points: %d"), Globals->MolePoints);
			}
		}
	}
}


