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

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* UnburrowSound = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* HitMole = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* HitVeggie = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* HitEmpty = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* PickUpVeggie = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* MolePickUpVeggie = nullptr;

    UPROPERTY(EditDefaultsOnly)
    class USoundBase* FarmerTaunt = nullptr;

    UPROPERTY(EditDefaultsOnly)
    class USoundBase* MoleTaunt = nullptr;

    UPROPERTY(EditDefaultsOnly)
    class USoundBase* HitGardener = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* StunnedMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UForceFeedbackEffect* ForceFeedbackEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 GameDuration = 60;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinScreenFarmer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinScreenMole = nullptr;

public:

	static const UWmGlobalsDataAsset* Get(const UObject* Context);
	
};
