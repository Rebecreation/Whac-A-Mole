#pragma once

struct FWmGlobals
{
    TWeakObjectPtr<const class UWmGlobalsDataAsset> GlobalsDataAsset = nullptr;

    int32 GardenerPoints = 0;
    int32 MolePoints = 0;

    TArray<TWeakObjectPtr<class AWmVeggieSpawner>> VeggieSpawners;

    static FWmGlobals* Get(const UObject* Context);
};
