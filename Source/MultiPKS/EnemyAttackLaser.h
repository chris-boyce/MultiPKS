// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAttackComp.h"
#include "EnemyAttackLaser.generated.h"

class UNiagaraComponent;
class UNiagaraSystem; 
UCLASS(Blueprintable)
class MULTIPKS_API UEnemyAttackLaser : public UBaseEnemyAttackComp
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere)
	UNiagaraComponent* LaserParticle; // Replicated particle system

	UPROPERTY(Replicated, Transient)
	FVector BeamStart;

	UPROPERTY(Replicated, Transient)
	FVector BeamEnd;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void BeginAttack(); 

	// Multicast function to start the effect on clients
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartLaserEffect(FVector NewBeamStart, FVector NewBeamEnd);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
