#include "WmGlobals.h"
#include "ArcECSSubsystem.h"
#include "../Resource/WmGlobals.h"

FWmGlobals* FWmGlobals::Get(const UObject* Context)
{
	if (!Context) { return nullptr; }
	UArcECSSubsystem* ECSSubsystem = UWorld::GetSubsystem<UArcECSSubsystem>(Context->GetWorld());
	if (!ECSSubsystem) { return nullptr; }
	FArcUniverse& Universe = ECSSubsystem->GetUniverse();
	if (FWmGlobals* Globals = Universe.GetResource<FWmGlobals>())
	{
		return Globals;
	}
	return nullptr;
};
