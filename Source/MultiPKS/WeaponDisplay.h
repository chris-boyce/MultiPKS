// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponDisplay.generated.h"

class UBarrelDisplay;
class UScopeDisplay;
class UMagDisplay;
UCLASS()
class MULTIPKS_API UWeaponDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText")
	TObjectPtr<UMagDisplay> BP_MagDisplay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText")
	TObjectPtr<UScopeDisplay> BP_ScopeDisplay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText")
	TObjectPtr<UBarrelDisplay> BP_BarrelDisplay = nullptr;

	
};
