#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WmGlobalsDataAsset.generated.h"

UCLASS()
class UWmGlobalsDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> GardenerClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> MoleClass = nullptr;
	
};
