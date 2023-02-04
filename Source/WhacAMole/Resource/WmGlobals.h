#pragma once

struct FWmGlobals
{
    TWeakObjectPtr<const class UWmGlobalsDataAsset> GlobalsDataAsset = nullptr;

    static FWmGlobals* Get(const UObject* Context);
};
