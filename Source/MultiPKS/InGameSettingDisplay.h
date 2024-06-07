// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsUtility.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "InGameSettingDisplay.generated.h"


UCLASS()
class MULTIPKS_API UInGameSettingDisplay : public UUserWidget
{
	GENERATED_BODY()

	/* --- Sensitivity --- */

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* INPUT_SEN;

	UPROPERTY(meta = (BindWidget))
	USpinBox* Bar_Sen;
	
	USettingsUtility* SettingsUtility;

	UFUNCTION()
	void SenChangeBar(float InValue);

	UFUNCTION()
	void SenChangeText(const FText& Text);

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* INPUT_ScopeSEN;

	UPROPERTY(meta = (BindWidget))
	USpinBox* Bar_ScopedSen;
	
	UFUNCTION()
	void SenScopeChangeBar(float InValue);

	UFUNCTION()
	void SenScopeChangeText(const FText& Text);
	
	UPROPERTY()
	float CurrentSense = 1.0f;

	UPROPERTY()
	float ScopedPercentage = 100.0f;

	/* --- Open and Close Of Menu --- */

	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/* ---  Anims ---  */
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* IntroAnim;
	
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Disconnect;
public:
	float GetCurrentSense() const {return  CurrentSense;}
	
	float GetScopedSensePercentage() const {return  ScopedPercentage;}

	

	
	

	
	
	

	
	
};
