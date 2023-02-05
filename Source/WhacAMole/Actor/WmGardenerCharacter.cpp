// Copyright Epic Games, Inc. All Rights Reserved.

#include "WmGardenerCharacter.h"
#include "WmMoleCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
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

AWmGardenerCharacter::AWmGardenerCharacter()
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

	HitBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(GetRootComponent());
}

void AWmGardenerCharacter::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (LastHitStartTime)
	{
		const float TimeSinceLastHit = UGameplayStatics::GetUnpausedTimeSeconds(this) - *LastHitStartTime;
		if (TimeSinceLastHit > HitCooldown)
		{
			LastHitStartTime.Reset();
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
	else if (StunImmunityStartTime)
	{
		const float TimeSinceAnimationStart = FMath::Max(0.0f, UGameplayStatics::GetUnpausedTimeSeconds(this) - *StunImmunityStartTime);
		if (TimeSinceAnimationStart > StunImmunityDuration)
		{
			StunImmunityStartTime.Reset();
		}
	}
}

void AWmGardenerCharacter::BeginPlay()
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
		Globals->Gardener = this;
	}
}

void AWmGardenerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
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

	PlayerInputComponent->BindAxis("MoveForward", this, &AWmGardenerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWmGardenerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Hit", IE_Released, this, &AWmGardenerCharacter::Hit);
	PlayerInputComponent->BindAction("PickUp", IE_Released, this, &AWmGardenerCharacter::TryPickUp);
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

void AWmGardenerCharacter::MoveForward(float Value)
{
	if (IsStunned()) { return; }
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

void AWmGardenerCharacter::MoveRight(float Value)
{
	if (IsStunned()) { return; }
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

void AWmGardenerCharacter::Hit()
{
	if (IsStunned()) { return; }
	if (LastHitStartTime) { return; }
	LastHitStartTime = UGameplayStatics::GetUnpausedTimeSeconds(this);
	ApplyHit();
    UGameplayStatics::PlaySoundAtLocation(this, GlobalsDataAsset->HitGardener, GetActorLocation(), GetActorRotation());
}

void AWmGardenerCharacter::ApplyHit()
{
	if (HitBox)
	{
		if (const UWmGlobalsDataAsset* GlobalsDataAsset = UWmGlobalsDataAsset::Get(this))
		{
			TArray<AActor*> OverlappingActors;
			HitBox->GetOverlappingActors(OverlappingActors);
			bool bHitMole = false;
			AWmVeggieSpawner* HitVeggie = nullptr;
			for (AActor* Actor : OverlappingActors)
			{
				AWmMoleCharacter* Mole = Cast<AWmMoleCharacter>(Actor);
				if (Mole && Mole->IsTargetable())
				{
					bHitMole = true;
					Mole->ApplyStun();
				}
				if (AWmVeggieSpawner* Veggie = Cast<AWmVeggieSpawner>(Actor))
				{
					HitVeggie = Veggie;
				}
			}
			if (bHitMole)
			{
				UGameplayStatics::PlaySoundAtLocation(this, GlobalsDataAsset->HitMole, GetActorLocation(), GetActorRotation());
			}
			else if (HitVeggie)
			{
				HitVeggie->ApplyHit();
				UGameplayStatics::PlaySoundAtLocation(this, GlobalsDataAsset->HitVeggie, GetActorLocation(), GetActorRotation());
			}
			else
			{
                float fPitch = FMath::RandRange(.5f,2.f);
                UGameplayStatics::PlaySoundAtLocation(this, GlobalsDataAsset->HitEmpty, GetActorLocation(), GetActorRotation(), 1.0f, fPitch);
            }
		}
	}
}

void AWmGardenerCharacter::ApplyStun()
{
	if (StunImmunityStartTime) { return; }

	StunStartTime = StunImmunityStartTime = UGameplayStatics::GetUnpausedTimeSeconds(this);
	SetStunnedMaterial(true);
}

bool AWmGardenerCharacter::IsStunned() const
{
	return StunStartTime.IsSet();
}

void AWmGardenerCharacter::TryPickUp()
{
	/*if (IsStunned()) { return; }
	if (HitBox)
	{
		FWmGlobals* Globals = FWmGlobals::Get(this);
		const UWmGlobalsDataAsset* GlobalsDataAsset = UWmGlobalsDataAsset::Get(this);
		if (Globals && GlobalsDataAsset)
		{
			TArray<AActor*> OverlappingActors;
			HitBox->GetOverlappingActors(OverlappingActors);
			for (AActor* Actor : OverlappingActors)
			{
				if (AWmVeggieSpawner* Veggie = Cast<AWmVeggieSpawner>(Actor))
				{
					int32 numPoints = Veggie->TryPick();
					if (numPoints != INDEX_NONE)
					{
						Globals->GardenerPoints += numPoints;
						UGameplayStatics::PlaySoundAtLocation(this, GlobalsDataAsset->PickUpVeggie, GetActorLocation(), GetActorRotation());
						UE_LOG(LogTemp, Warning, TEXT("Gardener Points: %d"), Globals->GardenerPoints);
					}
				}
			}
		}
	}*/

}

void AWmGardenerCharacter::SetStunnedMaterial(bool bStunned)
{
	FWmGlobals* Globals = FWmGlobals::Get(this);
	if (Globals && Globals->GlobalsDataAsset.IsValid())
	{
		TArray<UActorComponent*> MeshComponents = GetComponentsByClass(UStaticMeshComponent::StaticClass());
		for (UActorComponent* C : MeshComponents)
		{
			if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(C))
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
		TArray<UActorComponent*> SkMeshComponents = GetComponentsByClass(USkeletalMeshComponent::StaticClass());
		for (UActorComponent* C : SkMeshComponents)
		{

			if (USkeletalMeshComponent* MeshComponent = Cast<USkeletalMeshComponent>(C))
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


