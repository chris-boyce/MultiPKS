// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MagDisplay.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPKS_API UMagDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_MagName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_Rarity = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_Capacity = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText" ,meta = (BindWidget))
	UTextBlock* TXT_ReloadSpeed = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_Effect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_EffectChance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_ElementDuration = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void RunCompSetup();

	UFUNCTION()
	void SetAllText(FString Name, FString Rarity, int Capacity, float ReloadSpeed, FString Effect, float EffectChance, float EffectDuration);
	
};
