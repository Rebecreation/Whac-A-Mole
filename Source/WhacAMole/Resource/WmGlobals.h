#pragma once

struct FWmGlobals
{
    TWeakObjectPtr<const class UWmGlobalsDataAsset> GlobalsDataAsset = nullptr;

    TOptional<float> GameStartTime;

    int32 GardenerPoints = 0;
    int32 MolePoints = 0;

    TWeakObjectPtr<class AWmGardenerCharacter> Gardener = nullptr;
    TWeakObjectPtr<class AWmMoleCharacter> Mole = nullptr;

    TArray<TWeakObjectPtr<class AWmVeggieSpawner>> VeggieSpawners;

    static FWmGlobals* Get(const UObject* Context);
};
