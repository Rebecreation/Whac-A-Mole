#include "WmCoreSystems.h"
#include "EngineUtils.h"
#include "../DataAsset/WmGlobalsDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

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

void FWmCoreSystems::EvaluateGameOverCondition(FArcRes<FArcCoreData> CoreData, FArcRes<FWmGlobals> Globals)
{
	UWorld* World = CoreData->World.Get();
	APlayerController* PlayerController = World ? World->GetFirstPlayerController() : nullptr;
	const UWmGlobalsDataAsset* GlobalsDataAsset = Globals->GlobalsDataAsset.Get();
	if (!PlayerController || !GlobalsDataAsset || !Globals->GameStartTime || !GlobalsDataAsset->WinScreenFarmer || !GlobalsDataAsset->WinScreenMole) { return; }
	const bool bEndGame = [&]
	{
		const bool bDurationExceeded = UGameplayStatics::GetUnpausedTimeSeconds(World) - *Globals->GameStartTime > GlobalsDataAsset->GameDuration;
		if (bDurationExceeded)
		{
			if (Globals->GardenerPoints == Globals->MolePoints)
			{
				*Globals->GameStartTime += 15;
				return false;
			}
			return true;
		}
		return false;
	}();
	if (bEndGame && !Globals->WinScreen.IsValid())
	{
		UUserWidget* WinScreen = CreateWidget<UUserWidget>(PlayerController, Globals->GardenerPoints > Globals->MolePoints ? GlobalsDataAsset->WinScreenFarmer : GlobalsDataAsset->WinScreenMole);
		WinScreen->AddToViewport(1);
		PlayerController->SetShowMouseCursor(true);
		FInputModeGameAndUI Mode = FInputModeGameAndUI();
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(Mode);
		Globals->WinScreen = WinScreen;
	}
}

