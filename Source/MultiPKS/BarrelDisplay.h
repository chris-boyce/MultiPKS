// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "BarrelDisplay.generated.h"

enum class EFireMode : uint8;
class UTextBlock;

UCLASS()
class MULTIPKS_API UBarrelDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_BarrelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_FireRate = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_Damage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText" ,meta = (BindWidget))
	UTextBlock* TXT_FireMode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText" ,meta = (BindWidget))
	UTextBlock* TXT_BurstSpeed = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText" ,meta = (BindWidget))
	UTextBlock* TXT_BurstCount = nullptr;

	UFUNCTION()
	void SetAllText(FString BarrelName, float FireRate, float Damage, EFireMode FireMode, float BurstSpeed, int BurstCount);
	FString GetFireModeString(EFireMode FireMode) const;
};
