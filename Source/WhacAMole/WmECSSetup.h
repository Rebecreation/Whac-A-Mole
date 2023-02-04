#pragma once

#include "CoreMinimal.h"
#include "ArcECSSetup.h"
#include "WmECSSetup.generated.h"

UCLASS()
class UWmECSSetup : public UArcECSSetup
{
	GENERATED_BODY()

public:

	virtual void SetupECSForGame(FArcUniverse& Universe, UWorld& World, FArcScheduleBuilder& InitScheduleBuilder, FArcScheduleBuilder& TickScheduleBuilder) override;
};
