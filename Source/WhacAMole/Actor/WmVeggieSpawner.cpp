// Fill out your copyright notice in the Description page of Project Settings.

#include "WmVeggieSpawner.h"
#include "../Resource/WmGlobals.h"

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
	
	if (Veggies.Num() == 0) { return; }
	CurrentVeggieIndex = FMath::RandRange(0, Veggies.Num() - 1);
	FWmVeggieData& VeggieData = Veggies[*CurrentVeggieIndex];

	if (VeggieMesh)
	{
		VeggieMesh->SetStaticMesh(VeggieData.VeggieMesh);
		VeggieMesh->SetRelativeScale3D(FVector(VeggieData.MaxScale));
	}

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
	if (CurrentVeggieIndex)
	{
		VeggieMesh->SetVisibility(false);
	}
}

TOptional<int32> AWmVeggieSpawner::TryPick()
{
	if (CurrentVeggieIndex && VeggieMesh->IsVisible())
	{
		VeggieMesh->SetVisibility(false);
		const FWmVeggieData& VeggieData = Veggies[*CurrentVeggieIndex];
		return VeggieData.PointValue;
	}
	return {};
}

