// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem.h"

#include "Components/TextRenderComponent.h"

ASpawnSystem::ASpawnSystem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASpawnSystem::BeginPlay()
{
	Super::BeginPlay();
	if (Waves.Num() > 0)
	{
		CurrentWaveIndex = 0;
		StartNextWave(); 
	}
	
}

void ASpawnSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnSystem::StartNextWave()
{
	if(SpawnTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
	
	if (CurrentWaveIndex >= Waves.Num())
	{
		OnAllWavesCompleted.Broadcast();
		return; 
	}
	FWaveData waveData = Waves[CurrentWaveIndex];
	CurrentWaveEnemyCount = waveData.EnemyCount;
	EnemiesSpawned = 0;
	UpdateWaveText();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnSystem::SpawnEnemy, waveData.SpawnDelay, true);
}

void ASpawnSystem::SpawnEnemy()
{
	if (EnemiesSpawned >= CurrentWaveEnemyCount) 
	{
		return; 
	}

	FWaveData waveData = Waves[CurrentWaveIndex];
	int32 RandomIndex = FMath::RandRange(0, SpawnTransforms.Num() - 1);

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(waveData.EnemyClass, SpawnTransforms[RandomIndex], spawnParams);

	if (SpawnedEnemy)
	{
		SpawnedEnemy->OnDestroyed.AddDynamic(this, &ASpawnSystem::OnEnemyDestroyed);
		EnemiesSpawned++; 
	}
}

void ASpawnSystem::OnEnemyDestroyed(AActor* DestroyedEnemy)
{
	CurrentWaveEnemyCount--; 
	if (CurrentWaveEnemyCount <= 0 && EnemiesSpawned >= Waves[CurrentWaveIndex].EnemyCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		CurrentWaveIndex++;
		OnWaveCompleted.Broadcast();
		FTimerHandle TimerHandle;
		
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([this]()
		{
			StartNextWave();
		});
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 15.0f, false);
	}
	else
	{
		UTextRenderComponent* TextRenderer = FindComponentByClass<UTextRenderComponent>();
		TextRenderer->SetText(FText::FromString("Level Finished")); 
	}
	UpdateWaveText();
}

void ASpawnSystem::UpdateWaveText()
{
	UTextRenderComponent* TextRenderer = FindComponentByClass<UTextRenderComponent>(); 

	if (TextRenderer)
	{
		FString WaveText = FString::Printf(TEXT("Wave %d | Enemies: %d"), CurrentWaveIndex + 1, CurrentWaveEnemyCount); 
		TextRenderer->SetText(FText::FromString(WaveText)); 
	}
}

