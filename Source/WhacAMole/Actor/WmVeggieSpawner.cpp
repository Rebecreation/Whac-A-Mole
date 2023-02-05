// Fill out your copyright notice in the Description page of Project Settings.

#include "WmVeggieSpawner.h"
#include "../Resource/WmGlobals.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWmVeggieSpawner::AWmVeggieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	VeggieMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VeggieMesh"));
	VeggieMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AWmVeggieSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	/*if (Veggies.Num() == 0) { return; }
	CurrentVeggieIndex = FMath::RandRange(0, Veggies.Num() - 1);
	FWmVeggieData& VeggieData = Veggies[CurrentVeggieIndex];

	if (VeggieMesh)
	{
		VeggieMesh->SetStaticMesh(VeggieData.VeggieMesh);
		VeggieMesh->SetRelativeScale3D(FVector(VeggieData.MaxScale));
	}*/

	FWmGlobals* Globals = FWmGlobals::Get(this);
	if (ensure(Globals))
	{
		Globals->VeggieSpawners.Add(this);
	}
}

// Called every frame
void AWmVeggieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWmVeggieSpawner::ApplyHit()
{
	if (CurrentVeggieIndex != INDEX_NONE)
	{
		VeggieMesh->SetVisibility(false);
	}
}

void AWmVeggieSpawner::SetWobbleAmount(float WobbleAmount)
{
	if (WobbleAmount == 0.0f)
	{
		WobbleStartTime.Reset();
		VeggieMesh->SetWorldRotation(FQuat::Identity);
	}
	else
	{
		if (!WobbleStartTime && WobbleAmount > 0.0f)
		{
			WobbleStartTime = UGameplayStatics::GetUnpausedTimeSeconds(this);
		}
		if (VeggieMesh)
		{
			const float CurrentValue = FMath::Sin((UGameplayStatics::GetUnpausedTimeSeconds(this) - *WobbleStartTime) * WobbleSpeedScale) * WobbleAmount;
			VeggieMesh->SetWorldRotation(FQuat::MakeFromEuler(FVector(CurrentValue * 30.0f, 0.0f, 0.0f)));
		}
	}
}

int32 AWmVeggieSpawner::TryPick()
{
	if (CurrentVeggieIndex != INDEX_NONE && VeggieMesh->IsVisible())
	{
		VeggieMesh->SetVisibility(false);
		const FWmVeggieData& VeggieData = Veggies[CurrentVeggieIndex];
		CurrentVeggieIndex = INDEX_NONE;
		int32 Result = VeggieData.PointValue;
		if (FWmGlobals* Globals = FWmGlobals::Get(this))
		{
			Globals->GardenerPoints += Result;
		}
		OnPicked();
		return Result;
	}
	return INDEX_NONE;
}

int32 AWmVeggieSpawner::TryPickMole()
{
	if (CurrentVeggieIndex != INDEX_NONE && VeggieMesh->IsVisible())
	{
		VeggieMesh->SetVisibility(false);
		const FWmVeggieData& VeggieData = Veggies[CurrentVeggieIndex];
		CurrentVeggieIndex = INDEX_NONE;
		int32 Result = VeggieData.PointValue;
		OnPicked();
		return Result;
	}
	return INDEX_NONE;
}

void AWmVeggieSpawner::OnPicked_Implementation() {}

