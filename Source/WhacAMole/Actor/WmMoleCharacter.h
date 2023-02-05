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
	float BurrowDepth = 100.0f;

	UPROPERTY(EditAnywhere)
	float BurrowAnimationDuration = 0.2f;

	UPROPERTY(EditAnywhere)
	float UnburrowAnimationDuration = 0.5f;

	UPROPERTY(EditAnywhere)
	float MaxUndergroundDuration = 5.0f;

	UPROPERTY(EditAnywhere)
	float StunDuration = 2.0f;

	UPROPERTY(EditAnywhere)
	float FeedbackRadius = 200.0f;

	UPROPERTY(EditAnywhere)
	float PickUpRadius = 100.0f;

	UPROPERTY(EditAnywhere)
	float StunRadius = 100.0f;

	TOptional<float> BurrowAnimationStartTime;
	TOptional<float> UndergroundStartTime = 0.0f;
	TOptional<float> StunStartTime;

	bool bIsBurrowed = true;

	//bool bIsInsideForceFeedbackArea = false;

	UPROPERTY()
	TMap<class UStaticMeshComponent*, UMaterialInterface*> CachedMaterials;

	UPROPERTY()
	class AWmVeggieSpawner* ClosestVeggie = nullptr;

	//bool bWasInsideForceFeedbackArea = false;

	//UPROPERTY()
	//class AWmVeggieSpawner* CurrentlyWobblingVeggie = nullptr;

public:

	AWmMoleCharacter();

	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

	bool IsTargetable() const;
	bool IsStunned() const;

	void ApplyStun();
	
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
	void ToggleBurrowInternal();

	void TryPickUp();

	void SetStunnedMaterial(bool bStunned);

	class AWmVeggieSpawner* CalculateClosestVeggie(float MaxDistance) const;
};

