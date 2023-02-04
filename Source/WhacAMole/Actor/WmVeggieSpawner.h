// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WmVeggieSpawner.generated.h"

USTRUCT()
struct FWmVeggieData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* VeggieMesh = nullptr;

	UPROPERTY(EditAnywhere)
	int32 PointValue = 1;

	UPROPERTY(EditAnywhere)
	float MaxScale = 1.0f;
};

UCLASS()
class WHACAMOLE_API AWmVeggieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* VeggieMesh;

	UPROPERTY(EditAnywhere)
	float MaxVeggieScale = 1.0f;

	UPROPERTY(EditAnywhere)
	TArray<FWmVeggieData> Veggies;

	TOptional<int32> CurrentVeggieIndex;

	// Sets default values for this actor's properties
	AWmVeggieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyHit();
	TOptional<int32> TryPick();
};
