// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsUtility.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"

namespace
{
    const FString FileName = FString("Settings.json");

    TSharedPtr<FJsonObject> LoadSettings(const FString& FullPath)
    {
        FString JsonString;
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

        if (FFileHelper::LoadFileToString(JsonString, *FullPath))
        {
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
            FJsonSerializer::Deserialize(Reader, JsonObject);
        }

        return JsonObject;
    }

    void SaveSettings(const TSharedPtr<FJsonObject>& JsonObject, const FString& FullPath)
    {
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
        FFileHelper::SaveStringToFile(OutputString, *FullPath);
    }
}

void USettingsUtility::SaveFloatSetting(const FString& SettingName, float Value)
{
    FString SaveDirectory = FPaths::ProjectSavedDir();
    FString FullPath = SaveDirectory / FileName;
    TSharedPtr<FJsonObject> JsonObject = LoadSettings(FullPath);

    JsonObject->SetNumberField(SettingName, Value);
    SaveSettings(JsonObject, FullPath);
}

float USettingsUtility::LoadFloatSetting(const FString& SettingName, float DefaultValue)
{
    FString SaveDirectory = FPaths::ProjectSavedDir();
    FString FullPath = SaveDirectory / FileName;
    TSharedPtr<FJsonObject> JsonObject = LoadSettings(FullPath);

    if (JsonObject->HasField(SettingName))
    {
        return JsonObject->GetNumberField(SettingName);
    }

    return DefaultValue;
}

void USettingsUtility::SaveSensitivitySetting(float LookSensitivity)
{
    SaveFloatSetting("LookSensitivity", LookSensitivity);
}

float USettingsUtility::LoadSensitivitySetting()
{
    return LoadFloatSetting("LookSensitivity", 1.0f);
}

void USettingsUtility::SaveScopeSensitivitySetting(float LookSensitivity)
{
    SaveFloatSetting("ScopeSensitivity", LookSensitivity);
}

float USettingsUtility::LoadScopeSensitivitySetting()
{
    return LoadFloatSetting("ScopeSensitivity", 100.0f);
}

void USettingsUtility::SaveMusicVolume(float MusicVolume)
{
    SaveFloatSetting("MusicVolume", MusicVolume);
}

float USettingsUtility::LoadMusicVolume()
{
    return LoadFloatSetting("MusicVolume", 1.0f);
}

void USettingsUtility::SaveSFXVolume(float SFXVolume)
{
    SaveFloatSetting("SFXVolume", SFXVolume);
}

float USettingsUtility::LoadSFXVolume()
{
    return LoadFloatSetting("SFXVolume", 1.0f);
}
