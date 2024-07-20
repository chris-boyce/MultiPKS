// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blackhole.generated.h"

UCLASS()
class MULTIPKS_API ABlackhole : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlackhole();

protected:
	virtual void BeginPlay() override;

public:	
	float ElapsedTime = 0.0f;
	virtual void Tick(float DeltaTime) override;

};
