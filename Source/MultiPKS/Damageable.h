// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"


UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};


class MULTIPKS_API IDamageable
{
	GENERATED_BODY()
	
public:
	virtual void TakeDamage(float DamageAmount) = 0;
	
	virtual void DetailedTakeDamage(float DamageAmount, FVector HitLocation);

	virtual void DetailedTakeDamage2(float DamageAmount, FVector HitLocation, FName BoneName);
	
	virtual float GetHealth() const = 0;
};
