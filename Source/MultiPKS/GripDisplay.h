// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GripDisplay.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPKS_API UGripDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_GripName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_UnADSMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_ADSedMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_RecoilAmount;

	UFUNCTION()
	void SetAllText(FString GripName, float UnADSMovementSpeed, float ADSMovementSpeed, float RecoilAmount );
	
	
};
