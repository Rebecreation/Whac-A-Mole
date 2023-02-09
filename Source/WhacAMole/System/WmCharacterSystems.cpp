#include "WmCharacterSystems.h"
#include "Kismet/GameplayStatics.h"
#include "../DataAsset/WmGlobalsDataAsset.h"
#include "../Actor/WmGardenerCharacter.h"
#include "../Actor/WmMoleCharacter.h"
#include "GameFramework/Character.h"

void FWmCharacterSystems::CreateCharacters(FArcUniverse& Universe, FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals)
{
	//UWorld* World = CoreData->World.Get();

	//APlayerController* MoleController = UGameplayStatics::CreatePlayer(World, 1, true);
	//if (!MoleController) { return; }

	//if (APawn* DefaultPawn = MoleController->GetPawn())
	{
		//DefaultPawn->Destroy();
	}

	/*const UWmGlobalsDataAsset* GlobalsDataAsset = Globals->GlobalsDataAsset.Get();
	if (GlobalsDataAsset && GlobalsDataAsset->MoleClass)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACharacter* Mole = World->SpawnActor<ACharacter>(GlobalsDataAsset->MoleClass, FTransform::Identity, Params);
		MoleController->Possess(Mole);
	}*/
}

void FWmCharacterSystems::HandleInput(FArcRes<FWmGlobals> Globals, FArcRes<FWmCharacterEvents> Events)
{
	if (AWmGardenerCharacter* Gardener = Globals->Gardener.Get())
	{
		Gardener->MoveForward(Events->GardenerMovement.Y);
		Gardener->MoveRight(Events->GardenerMovement.X);
		if (Events->bGardenerHit)
		{
			Gardener->Hit();
		}
		if (Events->bGardenerTryPickUp)
		{
			Gardener->TryPickUp();
		}
	}
	if (AWmMoleCharacter* Mole = Globals->Mole.Get())
	{
		Mole->MoveForward(Events->MoleMovement.Y);
		Mole->MoveRight(Events->MoleMovement.X);
		if (Events->bMoleToggleBurrow)
		{
			Mole->ToggleBurrow();
		}
		if (Events->bMoleTryPickUp)
		{
			Mole->TryPickUp();
		}
	}
}

void FWmCharacterSystems::ClearEvents(FArcRes<FWmCharacterEvents> Events)
{
	*Events = FWmCharacterEvents();
}
