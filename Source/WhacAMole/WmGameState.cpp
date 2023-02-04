// Fill out your copyright notice in the Description page of Project Settings.

#include "WmGameState.h"
#include "Resource/WmGlobals.h"

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

