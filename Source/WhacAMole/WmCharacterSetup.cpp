#include "WmCharacterSetup.h"
#include "ArcUniverse.h"
#include "ArcScheduleBuilder.h"
#include "System/WmCharacterSystems.h"

void FWmCharacterSetup::Setup(FArcUniverse& Universe, FArcScheduleBuilder& InitScheduleBuilder, FArcScheduleBuilder& TickScheduleBuilder)
{
	Universe.AddResource(FWmCharacterEvents());
	
	InitScheduleBuilder
		.AddSystem(&FWmCharacterSystems::CreateCharacters);

	TickScheduleBuilder
		.AddSystemToStage(TEXT("Wm_EventCleanUpStage"), &FWmCharacterSystems::ClearEvents)
		.AddSystem(&FWmCharacterSystems::HandleInput);
}
