// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "GameFramework/Actor.h"
#include "SpawnSystem.generated.h"

class UTextRenderComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllWavesCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaveCompleted);

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TSubclassOf<class ABaseEnemy> EnemyClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemyCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float SpawnDelay;
	
	
};

UCLASS()
class MULTIPKS_API ASpawnSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="WaveSpawner")
	TSubclassOf<ABaseEnemy> BaseEnemyClass;

	UPROPERTY(BlueprintAssignable, Category = "Wave")
	FOnAllWavesCompleted OnAllWavesCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
	TArray<FWaveData> Waves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave", meta=(MakeEditWidget="true", AllowPrivateAccess="true"))
	TArray<FTransform> SpawnTransforms;

	UPROPERTY(BlueprintAssignable)
	FOnWaveCompleted OnWaveCompleted;


protected:
	FTimerHandle SpawnTimerHandle;
	
	int32 CurrentWaveIndex = 0;
	
	int32 CurrentWaveEnemyCount = 0;

	int EnemiesSpawned = 0;
	
	
	void StartNextWave();

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	void OnEnemyDestroyed(AActor* DestroyedEnemy);

	UFUNCTION()
	void UpdateWaveText();
	
	

	

};
