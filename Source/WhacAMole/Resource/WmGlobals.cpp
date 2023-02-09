#include "WmGlobals.h"
#include "ArcECSSubsystem.h"

FWmGlobals* FWmGlobals::Get(const UObject* Context)
{
	if (!Context) { return nullptr; }
	UArcECSSubsystem* ECSSubsystem = UWorld::GetSubsystem<UArcECSSubsystem>(Context->GetWorld());
	if (!ECSSubsystem) { return nullptr; }
	FArcUniverse& Universe = ECSSubsystem->GetUniverse();
	return Universe.GetResource<FWmGlobals>();
};
