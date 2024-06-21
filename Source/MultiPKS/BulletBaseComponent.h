// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPKS_API UBulletBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBulletBaseComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void HandleImpact(const FHitResult& HitResult);
	
	virtual void HandleMovement();
};
