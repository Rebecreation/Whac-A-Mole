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
}

void AWmMoleCharacter::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	float AnimationRatio = bIsBurrowed ? 1.0f : 0.0f;

	if (BurrowAnimationStartTime)
	{
		const float TimeSinceAnimationStart = FMath::Max(0.0f, UGameplayStatics::GetUnpausedTimeSeconds(this) - *BurrowAnimationStartTime);
		const float Ratio = TimeSinceAnimationStart / BurrowAnimationDuration;

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

	UE_LOG(LogTemp, Warning, TEXT("%f"), AnimationRatio)

	if (MoleRoot)
	{
		MoleRoot->SetRelativeLocation(FVector::DownVector * AnimationRatio * BurrowDepth);
	}
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
	if (!bIsBurrowed) { return;}
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
	if (!bIsBurrowed) { return;}
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
	if (!BurrowAnimationStartTime)
	{
		bIsBurrowed = !bIsBurrowed;
		BurrowAnimationStartTime = UGameplayStatics::GetTimeSeconds(this);
	}
}

