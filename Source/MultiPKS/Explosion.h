// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

UCLASS()
class MULTIPKS_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosion();
	
	UPROPERTY(EditAnywhere, Category="Bullet")
	UParticleSystemComponent* ExplosionEffect;
	
	UPROPERTY(EditAnywhere, Category="Bullet")
	UAudioComponent* ExplosionSound;
	float DamageRadius = 100.0f;
	float ExplosionDamage = 50.0f;

	void InitExplosion(float NewDamageRadius, float NewExplosionDamage);


protected:
	float CalculateDamage(float Distance);
	
	virtual void BeginPlay() override;
	void Explode();

public:	
	virtual void Tick(float DeltaTime) override;

};
