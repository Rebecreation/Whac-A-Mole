#include "WmCoreSetup.h"
#include "ArcUniverse.h"
#include "ArcScheduleBuilder.h"
#include "Resource/WmGlobals.h"
#include "System/WmCoreSystems.h"

void FWmCoreSetup::Setup(FArcUniverse& Universe, FArcScheduleBuilder& InitScheduleBuilder, FArcScheduleBuilder& TickScheduleBuilder)
{
	Universe.AddResource(FWmGlobals());

	InitScheduleBuilder
		.AddSystem(&FWmCoreSystems::LoadGlobals);
}
