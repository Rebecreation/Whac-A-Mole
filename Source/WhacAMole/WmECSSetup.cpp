#include "WmECSSetup.h"
#include "ArcScheduleBuilder.h"
#include "WmCharacterSetup.h"
#include "WmCoreSetup.h"

void UWmECSSetup::SetupECSForGame(FArcUniverse& Universe, UWorld& World, FArcScheduleBuilder& InitScheduleBuilder, FArcScheduleBuilder& TickScheduleBuilder)
{
	TickScheduleBuilder
		.AddUnrealStages()
		.AddStageAfter(TEXT("Wm_EventCleanUpStage"), FArcScheduleStage::LastDemotableStage);

	FWmCoreSetup::Setup(Universe, InitScheduleBuilder, TickScheduleBuilder);
	FWmCharacterSetup::Setup(Universe, InitScheduleBuilder, TickScheduleBuilder);
}
