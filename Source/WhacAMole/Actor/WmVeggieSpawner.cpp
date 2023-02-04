// Fill out your copyright notice in the Description page of Project Settings.


#include "WmVeggieSpawner.h"

// Sets default values
AWmVeggieSpawner::AWmVeggieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWmVeggieSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWmVeggieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

