// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WmVeggieSpawner.generated.h"

USTRUCT(BlueprintType)
struct FWmVeggieData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* VeggieMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 PointValue = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxScale = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D DormantTime = { 1.f, 5.f };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D AutopickTime =  { 1.f, 5.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D DisableTime = { 1.f, 5.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GrowSpeed = 0.1f;
};

UCLASS()
class WHACAMOLE_API AWmVeggieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* VeggieMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FWmVeggieData> Veggies;

	UPROPERTY(Transient, BlueprintReadWrite)
	int32 CurrentVeggieIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
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

	UFUNCTION(BlueprintCallable)
	void SetWobbleAmount(float WobbleAmount);

	UFUNCTION(BlueprintCallable)
	int32 TryPick();

	int32 TryPickMole();

	UFUNCTION(BlueprintNativeEvent)
	void OnPicked();

	UFUNCTION(BlueprintNativeEvent)
	void OnDisabled();
	UFUNCTION(BlueprintNativeEvent)
	void OnReenable();
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bIsDisabled = false;
	UPROPERTY(Transient, BlueprintReadWrite)
	bool bIsActive = false;
};
