// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBaseComponent.h"
#include "BulletBounceComp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPKS_API UBulletBounceComp : public UBulletBaseComponent
{
	GENERATED_BODY()
public:
	UBulletBounceComp();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void HandleImpact(const FHitResult& HitResult) override;

	bool HasBounced = false;
	
};
