#include "WmCoreSystems.h"
#include "EngineUtils.h"
#include "../DataAsset/WmGlobalsDataAsset.h"

void FWmCoreSystems::LoadGlobals(FArcUniverse& Universe, FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals)
{
	UWorld* World = CoreData->World.Get();
	TArray<UObject*> Assets;
	EngineUtils::FindOrLoadAssetsByPath(TEXT("/Game/Globals"), Assets, EngineUtils::ATL_Regular);
	for (auto Asset : Assets)
	{
		if (UWmGlobalsDataAsset* GlobalsDataAsset = Cast<UWmGlobalsDataAsset>(Asset))
		{
			Globals->GlobalsDataAsset = GlobalsDataAsset;
		}
	}
}

