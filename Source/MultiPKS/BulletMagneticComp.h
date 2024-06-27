// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBaseComponent.h"
#include "Components/SphereComponent.h"
#include "BulletMagneticComp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPKS_API UBulletMagneticComp : public UBulletBaseComponent
{
	GENERATED_BODY()
public:
	UBulletMagneticComp();

protected:
	virtual void BeginPlay() override;

public:
	
	virtual void HandleMovement() override;

	virtual bool HandleImpact(const FHitResult& HitResult) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	bool IsValidTarget(AActor* OtherActor) const;

	USphereComponent* DetectionSphere;
	
	AActor* CurrentTarget;
};
