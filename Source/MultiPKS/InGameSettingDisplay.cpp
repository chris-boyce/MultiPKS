// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameSettingDisplay.h"

#include "GameFramework/GameUserSettings.h"
#include "Misc/DefaultValueHelper.h"


void UInGameSettingDisplay::ResChangeDown()
{
	CurrentSense = CurrentSense - 1;
	TXT_Res->SetText(FText::Format(FText::FromString("{0} x {1}"), FText::AsNumber(ResolutionVector[CurrentResSelection].X), FText::AsNumber(ResolutionVector[CurrentResSelection].Y)));
	ChangeRes();	
	UE_LOG(LogTemp, Error, TEXT("Res Going Down"));
	
}

void UInGameSettingDisplay::ResChangeUp()
{
	CurrentSense = CurrentSense + 1;
	TXT_Res->SetText(FText::Format(FText::FromString("{0} x {1}"), FText::AsNumber(ResolutionVector[CurrentResSelection].X), FText::AsNumber(ResolutionVector[CurrentResSelection].Y)));
	ChangeRes();	
	UE_LOG(LogTemp, Error, TEXT("Res Going Up"));
	
}

void UInGameSettingDisplay::ChangeRes()
{
	UE_LOG(LogTemp, Error, TEXT("Trying to Change Res"));
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	
	if (UserSettings)
	{
		UE_LOG(LogTemp, Error, TEXT("Has Changed Res"));
		FIntPoint Resolution(ResolutionVector[CurrentResSelection].X, ResolutionVector[CurrentResSelection].Y);
		UserSettings->SetScreenResolution(Resolution);
		UserSettings->ApplySettings(false);
	}
}

void UInGameSettingDisplay::NativeConstruct()
{
	Super::NativeConstruct();
	ResolutionVector.Add(FVector2D(1280, 720));
	ResolutionVector.Add(FVector2D(1280, 720));
	ResolutionVector.Add(FVector2D(1600, 900));
	ResolutionVector.Add(FVector2D(2560, 1440));
	
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
	if(BTN_ResDown)
	{
		BTN_ResDown->OnClicked.AddDynamic(this, &UInGameSettingDisplay::ResChangeDown);
	}
	if(BTN_ResUp)
	{
		BTN_ResUp->OnClicked.AddDynamic(this, &UInGameSettingDisplay::ResChangeUp);
	}

	
}

void UInGameSettingDisplay::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT("ClosedMenu"));Super::NativeDestruct();
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

void UInGameSettingDisplay::SenChangeBar(float InValue)
{
	CurrentSense = InValue;
	INPUT_SEN->SetText(FText::AsNumber(InValue));
}

