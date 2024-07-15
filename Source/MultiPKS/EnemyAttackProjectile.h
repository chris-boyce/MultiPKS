// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAttackComp.h"
#include "EnemyAttackProjectile.generated.h"

class ABullet;
/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPKS_API UEnemyAttackProjectile : public UBaseEnemyAttackComp
{
	GENERATED_BODY()
public:
	
	
	UFUNCTION(BlueprintCallable)
	virtual void BeginAttack() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_BeginAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnBullet();


	UFUNCTION(Client, Reliable)
	void Client_PredictAttack();



	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
