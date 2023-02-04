// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WmGameState.generated.h"

/**
 * 
 */
UCLASS()
class WHACAMOLE_API AWmGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	int32 GetGardenerPoints() const;

	UFUNCTION(BlueprintCallable)
	int32 GetMolePoints() const;
	
};
