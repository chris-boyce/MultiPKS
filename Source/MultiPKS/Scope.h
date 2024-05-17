// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scope.generated.h"

UCLASS()
class MULTIPKS_API AScope : public AActor
{
	GENERATED_BODY()
	
public:	
	AScope();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Scope|ADS")
	float ADSSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="Scope|ADS")
	TSubclassOf<UUserWidget> ScopeWidget;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Scope|ADS")
	float FOVChange = 50;

	
	

};
