// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagDisplay.h"
#include "Blueprint/UserWidget.h"
#include "WeaponDisplay.generated.h"


UCLASS()
class MULTIPKS_API UWeaponDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText")
	TObjectPtr<UMagDisplay> BP_MagDisplay = nullptr;

	
};
