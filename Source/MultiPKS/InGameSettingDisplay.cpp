// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameSettingDisplay.h"

#include "SettingsUtility.h"
#include "GameFramework/GameUserSettings.h"
#include "Misc/DefaultValueHelper.h"


void UInGameSettingDisplay::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (IntroAnim)
	{
		PlayAnimation(IntroAnim);
	}
	if(INPUT_SEN)
	{
		INPUT_SEN->OnTextChanged.AddDynamic(this, &UInGameSettingDisplay::SenChangeText);
	}
	if(Bar_Sen)
	{
		Bar_Sen->OnValueChanged.AddDynamic(this, &UInGameSettingDisplay::SenChangeBar);
	}
	if(INPUT_ScopeSEN)
	{
		INPUT_ScopeSEN->OnTextChanged.AddDynamic(this, &UInGameSettingDisplay::SenScopeChangeText);
	}
	if(Bar_ScopedSen)
	{
		Bar_ScopedSen->OnValueChanged.AddDynamic(this, &UInGameSettingDisplay::SenScopeChangeBar);
	}
	SettingsUtility = NewObject<USettingsUtility>();
	Bar_Sen->SetValue(SettingsUtility->LoadSensitivitySetting());
	INPUT_SEN->SetText(FText::AsNumber(SettingsUtility->LoadSensitivitySetting()));
	Bar_ScopedSen->SetValue(SettingsUtility->LoadScopeSensitivitySetting());
	INPUT_ScopeSEN->SetText(FText::AsNumber(SettingsUtility->LoadScopeSensitivitySetting()));

	
}

void UInGameSettingDisplay::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT("ClosedMenu"));
	Super::NativeDestruct();
}

void UInGameSettingDisplay::SenChangeText(const FText& Text) /* Called When The TEXT input is changed */
{
	float NewSense;
	if (FDefaultValueHelper::ParseFloat(Text.ToString(), NewSense))
	{
		CurrentSense = NewSense;
		Bar_Sen->SetValue(CurrentSense);
	}
	else
	{
		INPUT_SEN->SetText(FText::AsNumber(CurrentSense));
	}
	
}

void UInGameSettingDisplay::SenScopeChangeBar(float InValue)
{
	ScopedPercentage = InValue;
	INPUT_ScopeSEN->SetText(FText::AsNumber(InValue));
}

void UInGameSettingDisplay::SenScopeChangeText(const FText& Text)
{
	float NewSense;
	if (FDefaultValueHelper::ParseFloat(Text.ToString(), NewSense))
	{
		ScopedPercentage = NewSense;
		Bar_ScopedSen->SetValue(ScopedPercentage);
	}
	else
	{
		INPUT_ScopeSEN->SetText(FText::AsNumber(ScopedPercentage));
	}
}

void UInGameSettingDisplay::SenChangeBar(float InValue)
{
	CurrentSense = InValue;
	INPUT_SEN->SetText(FText::AsNumber(InValue));
}

