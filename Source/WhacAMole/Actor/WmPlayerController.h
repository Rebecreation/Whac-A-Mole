// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WmPlayerController.generated.h"

UCLASS()
class WHACAMOLE_API AWmPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	virtual void SetupInputComponent() override;

private:

	void GardenerMoveForward(float Value);
	void GardenerMoveRight(float Value);
	void GardenerHit();
	void GardenerTryPickUp();

	void MoleMoveForward(float Value);
	void MoleMoveRight(float Value);
	void MoleToggleBurrow();
	void MoleTryPickUp();
};
