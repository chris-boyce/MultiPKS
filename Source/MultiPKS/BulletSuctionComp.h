// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBaseComponent.h"
#include "BulletSuctionComp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPKS_API UBulletSuctionComp : public UBulletBaseComponent
{
	GENERATED_BODY()
public:
	UBulletSuctionComp();
protected:
	virtual void BeginPlay() override;
public:
	virtual void HandleImpact(const FHitResult& HitResult) override;

	void ApplySuction(const FVector& ImpactPoint);
	
};
