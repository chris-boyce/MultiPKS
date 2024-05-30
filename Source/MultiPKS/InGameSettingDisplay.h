// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	UFUNCTION()
	void SenChangeBar(float InValue);

	UFUNCTION()
	void SenChangeText(const FText& Text);
	
	UPROPERTY()
	float CurrentSense = 1.0f;

	/* --- Open and Close Of Menu --- */

	UFUNCTION()
	void ResChangeDown();

	UFUNCTION()
	void ResChangeUp();

	UFUNCTION()
	void ChangeRes();
	
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	/* ---  Anims ---  */
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* IntroAnim;

	/* --- Resolution --- */

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_ResDown;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_ResUp;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TXT_Res;

	int CurrentResSelection = 0;

	TArray<FVector2D> ResolutionVector;

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Disconnect;

	

	
	

	
	
	

	
	
};
