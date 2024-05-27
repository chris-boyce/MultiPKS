// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarDisplay.generated.h"

class UTextBlock;
class UProgressBar;
class UBorder;
/**
 * 
 */
UCLASS()
class MULTIPKS_API UHealthBarDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBorder* BloodBorderZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UProgressBar* HealthBarZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextBlock* HealthTextZ;

	UFUNCTION()
	void UpdateHealth(float CurrentHealth, float MaxHealth);
private:
	FTimerHandle OpacityTimerHandle;
	float CurrentOpacity = 0;
	float TargetOpacity = 0;
	float FadeSpeed = 0.5f;

	void UpdateOpacity();

	virtual void NativeConstruct() override;
	
	
};
