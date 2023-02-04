#include "WmCharacterSetup.h"
#include "ArcUniverse.h"
#include "ArcScheduleBuilder.h"
#include "System/WmCharacterSystems.h"

void FWmCharacterSetup::Setup(FArcUniverse& Universe, FArcScheduleBuilder& InitScheduleBuilder, FArcScheduleBuilder& TickScheduleBuilder)
{
	InitScheduleBuilder
		.AddSystem(&FWmCharacterSystems::CreateCharacters);
}
