#pragma once

#include "ArcRes.h"
#include "Resource/ArcCoreData.h"
#include "../Resource/WmGlobals.h"

struct FWmCoreSystems
{
	// Init
	static void LoadGlobals(class FArcUniverse& Universe, FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals);

	// Tick
	static void EvaluateGameOverCondition(FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals);
};
