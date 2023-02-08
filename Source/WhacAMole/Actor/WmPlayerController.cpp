// Fill out your copyright notice in the Description page of Project Settings.

#include "WmPlayerController.h"
#include "../Resource/WmCharacterEvents.h"

void AWmPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetShowMouseCursor(true);

	InputComponent->BindAxis("GardenerMoveForward", this, &AWmPlayerController::GardenerMoveForward);
	InputComponent->BindAxis("GardenerMoveRight", this, &AWmPlayerController::GardenerMoveRight);

	InputComponent->BindAction("GardenerHit", IE_Released, this, &AWmPlayerController::GardenerHit);
	InputComponent->BindAction("GardenerPickUp", IE_Released, this, &AWmPlayerController::GardenerTryPickUp);
	
	InputComponent->BindAxis("MoleMoveForward", this, &AWmPlayerController::MoleMoveForward);
	InputComponent->BindAxis("MoleMoveRight", this, &AWmPlayerController::MoleMoveRight);

	InputComponent->BindAction("MoleToggleBurrow", IE_Released, this, &AWmPlayerController::MoleToggleBurrow);
	InputComponent->BindAction("MolePickUp", IE_Released, this, &AWmPlayerController::MoleTryPickUp);
}

void AWmPlayerController::GardenerMoveForward(float Value)
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->GardenerMovement.Y = Value;
	}
}

void AWmPlayerController::GardenerMoveRight(float Value)
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->GardenerMovement.X = Value;
	}
}

void AWmPlayerController::GardenerHit()
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->bGardenerHit = true;
	}
}

void AWmPlayerController::GardenerTryPickUp()
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->bGardenerTryPickUp = true;
	}
}

void AWmPlayerController::MoleMoveForward(float Value)
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->MoleMovement.Y = Value;
	}
}

void AWmPlayerController::MoleMoveRight(float Value)
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->MoleMovement.X = Value;
	}
}

void AWmPlayerController::MoleToggleBurrow()
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->bMoleToggleBurrow = true;
	}
}

void AWmPlayerController::MoleTryPickUp()
{
	if (FWmCharacterEvents* CharacterEvents = FWmCharacterEvents::Get(this))
	{
		CharacterEvents->bMoleTryPickUp = true;
	}
}
