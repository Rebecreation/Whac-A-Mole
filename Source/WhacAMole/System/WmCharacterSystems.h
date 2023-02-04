#pragma once

#include "ArcRes.h"
#include "Resource/ArcCoreData.h"
#include "../Resource/WmGlobals.h"

struct FWmCharacterSystems
{
	// Init
	static void CreateCharacters(class FArcUniverse& Universe, FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals);

	// Tick
	//static void ClearEvents(FArcRes<FWmCharacterEvents> Events);
	
};
