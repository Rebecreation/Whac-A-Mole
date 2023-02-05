// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WmVeggieSpawner.generated.h"

USTRUCT()
struct FWmVeggieData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* VeggieMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 PointValue = 1;

	UPROPERTY(EditDefaultsOnly)
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

	UPROPERTY(EditDefaultsOnly)
	TArray<FWmVeggieData> Veggies;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 CurrentVeggieIndex;

	UPROPERTY(EditAnywhere)
	float WobbleSpeedScale = 2.0f;

	TOptional<float> WobbleStartTime;

	// Sets default values for this actor's properties
	AWmVeggieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyHit();

	void SetWobbleAmount(float WobbleAmount);

	UFUNCTION(BlueprintCallable)
	int32 TryPick();

	UFUNCTION(BlueprintNativeEvent)
	void OnPicked();
};
