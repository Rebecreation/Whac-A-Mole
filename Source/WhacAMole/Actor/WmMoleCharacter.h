// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WmMoleCharacter.generated.h"


UCLASS(config=Game)
class AWmMoleCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	class USceneComponent* MoleRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere)
	float BurrowDepth = 100.0f;

	UPROPERTY(EditAnywhere)
	float BurrowAnimationDuration = 0.2f;

	TOptional<float> BurrowAnimationStartTime;

	bool bIsBurrowed = true;

public:

	AWmMoleCharacter();

	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	
protected:

	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Called for movement input */
	//void Move(const FInputActionValue& Value);

	/** Called for looking input */
	//void Look(const FInputActionValue& Value);

	void MoveForward(float Value);
	void MoveRight(float Value);

	void ToggleBurrow();

};

