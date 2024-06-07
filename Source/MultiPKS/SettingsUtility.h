// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingsUtility.generated.h"


UCLASS()
class MULTIPKS_API USettingsUtility : public UObject
{
	GENERATED_BODY()
public:
	
	
	void SaveFloatSetting(const FString& SettingName, float Value);

	
	float LoadFloatSetting(const FString& SettingName, float DefaultValue);

	
	void SaveSensitivitySetting(float LookSensitivity);
	float LoadSensitivitySetting();
	
	void SaveScopeSensitivitySetting(float LookSensitivity);
	float LoadScopeSensitivitySetting();

	void SaveMusicVolume(float MusicVolume);
	float LoadMusicVolume();

	void SaveSFXVolume(float SFXVolume);
	float LoadSFXVolume();
	
	
};
