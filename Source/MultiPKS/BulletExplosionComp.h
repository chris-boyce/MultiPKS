// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBaseComponent.h"
#include "Explosion.h"
#include "BulletExplosionComp.generated.h"


UCLASS(Blueprintable)
class MULTIPKS_API UBulletExplosionComp : public UBulletBaseComponent
{
	GENERATED_BODY()

public:
	UBulletExplosionComp();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AExplosion> ExplosiveClass;

	virtual void HandleImpact(const FHitResult& HitResult) override;

private:
	void Explode(const FVector& HitLocation);
	
};
