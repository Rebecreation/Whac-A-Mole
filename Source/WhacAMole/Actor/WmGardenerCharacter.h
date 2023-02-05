// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WmGardenerCharacter.generated.h"

UENUM()
enum class EWmGardenerState : uint8
{
	Default,
	Attacking
};

UCLASS(config=Game)
class AWmGardenerCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* HitBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere)
	float HitCooldown = 1.0f;

	UPROPERTY(EditAnywhere)
	float StunDuration = 2.0f;

	UPROPERTY(EditAnywhere)
	float StunImmunityDuration = 4.0f;

	UPROPERTY(EditAnywhere)
		float FeedbackRadius = 200.0f;

	TOptional<float> LastHitStartTime;

	TOptional<float> StunStartTime;
	TOptional<float> StunImmunityStartTime;

	UPROPERTY()
	TMap<class UMeshComponent*, UMaterialInterface*> CachedMaterials;

public:

	AWmGardenerCharacter();

	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	void ApplyStun();
	bool IsStunned() const;
	
protected:

	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Called for movement input */
	//void Move(const FInputActionValue& Value);

	/** Called for looking input */
	//void Look(const FInputActionValue& Value);

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Hit();

	void ApplyHit();

	void TryPickUp();

	void SetStunnedMaterial(bool bStunned);
};

