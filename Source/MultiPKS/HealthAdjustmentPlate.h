// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthAdjustmentPlate.generated.h"

UCLASS()
class MULTIPKS_API AHealthAdjustmentPlate : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealthAdjustmentPlate();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="EffectAmount")
	float Amount = 10;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	
};
