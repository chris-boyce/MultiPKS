// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerAmmoHUD.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MULTIPKS_API UPlayerAmmoHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="AmmoText", meta = (BindWidget))
	UTextBlock* TXT_CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="AmmoText", meta = (BindWidget))
	UTextBlock* TXT_MaxAmmo;

	UFUNCTION()
	void SetAmmoText(int CurrentAmmo, int MaxAmmo);
	
};
