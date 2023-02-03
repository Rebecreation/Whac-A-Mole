// Copyright Epic Games, Inc. All Rights Reserved.

#include "WhacAMoleGameMode.h"
#include "WhacAMoleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWhacAMoleGameMode::AWhacAMoleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
