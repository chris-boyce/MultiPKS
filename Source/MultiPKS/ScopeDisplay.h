// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScopeDisplay.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPKS_API UScopeDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_ScopeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_ADSSpeed= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_ZoomAmount = nullptr;

	UFUNCTION()
	float MagnificationCalculation(float FOV);

	UFUNCTION()
	void SetAllText(FString Name, float ADSSpeed, float FOV);
	
	
	
};
