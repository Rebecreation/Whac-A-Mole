#include "WmCharacterSystems.h"
#include "Kismet/GameplayStatics.h"
#include "../DataAsset/WmGlobalsDataAsset.h"
#include "GameFramework/Character.h"

void FWmCharacterSystems::CreateCharacters(FArcUniverse& Universe, FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals)
{
	UWorld* World = CoreData->World.Get();

	APlayerController* MoleController = UGameplayStatics::CreatePlayer(World, 1, true);
	if (!MoleController) { return; }

	if (APawn* DefaultPawn = MoleController->GetPawn())
	{
		DefaultPawn->Destroy();
	}

	const UWmGlobalsDataAsset* GlobalsDataAsset = Globals->GlobalsDataAsset.Get();
	if (GlobalsDataAsset && GlobalsDataAsset->MoleClass)
	{
		ACharacter* Mole = World->SpawnActor<ACharacter>(GlobalsDataAsset->MoleClass, FTransform::Identity);
		MoleController->Possess(Mole);
	}
}

/*void FWmCharacterSystems::ClearEvents(FArcRes<FWmCharacterEvents> Events)
{
	*Events = FWmCharacterEvents();
}*/
