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
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void BeginAttack() override;

	UPROPERTY(Replicated, EditAnywhere)
	UNiagaraComponent* LaserParticle;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
