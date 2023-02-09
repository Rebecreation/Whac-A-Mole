#pragma once

#include "ArcRes.h"
#include "Resource/ArcCoreData.h"
#include "../Resource/WmCharacterEvents.h"
#include "../Resource/WmGlobals.h"

struct FWmCharacterSystems
{
	// Init
	static void CreateCharacters(class FArcUniverse& Universe, FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals);

	// Tick
	static void HandleInput(FArcRes<FWmGlobals> Globals, FArcRes<FWmCharacterEvents> Events);
	static void ClearEvents(FArcRes<FWmCharacterEvents> Events);
	
};
