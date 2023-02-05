// Fill out your copyright notice in the Description page of Project Settings.

#include "WmGameState.h"
#include "Resource/WmGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "DataAsset/WmGlobalsDataAsset.h"

int32 AWmGameState::GetGardenerPoints() const
{
	if (FWmGlobals* Globals = FWmGlobals::Get(this))
	{
		return Globals->GardenerPoints;
	}
	return 0;
}

int32 AWmGameState::GetMolePoints() const
{
	if (FWmGlobals* Globals = FWmGlobals::Get(this))
	{
		return Globals->MolePoints;
	}
	return 0;
}

void AWmGameState::ResetLevel()
{
	if (FWmGlobals* Globals = FWmGlobals::Get(this))
	{
		Globals->GameStartTime = UGameplayStatics::GetUnpausedTimeSeconds(this);
		Globals->GardenerPoints = 0;
		Globals->MolePoints = 0;
	}
}

FString AWmGameState::GetTimerString() const
{
	FWmGlobals* Globals = FWmGlobals::Get(this);
	const UWmGlobalsDataAsset* GlobalsDataAsset = Globals ? Globals->GlobalsDataAsset.Get() : nullptr;
	if (Globals && GlobalsDataAsset && Globals->GameStartTime)
	{
		int32 Seconds = GlobalsDataAsset->GameDuration - (UGameplayStatics::GetUnpausedTimeSeconds(this) - *Globals->GameStartTime);
		const int32 Minutes = Seconds / 60;
		Seconds = FMath::Max(0, Seconds % 60);

		return FString::Printf(TEXT("%d:%02d"), Minutes, Seconds);
	}
	return FString();
}


