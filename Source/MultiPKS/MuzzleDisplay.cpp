// Fill out your copyright notice in the Description page of Project Settings.


#include "MuzzleDisplay.h"
#include "Muzzle.h"
#include "Components/TextBlock.h"

void UMuzzleDisplay::SetAllText(FString MuzzleName, ESoundLevel SoundLevel, float BulletVelocity)
{
	TXT_MuzzleName->SetText(FText::FromString(MuzzleName));
	TXT_SoundLevel->SetText(FText::FromString(GetSoundLevelString(SoundLevel)));
	TXT_BulletVelocity->SetText(FText::FromString(FString::Printf(TEXT("%.2fm/s"), BulletVelocity / 1000)));
	
}

FString UMuzzleDisplay::GetSoundLevelString(ESoundLevel SoundLevel) const
{
	switch (SoundLevel)
	{
	case ESoundLevel::Silenced:
		return TEXT("Silenced");
	case ESoundLevel::Medium:
		return TEXT("Medium");
	case ESoundLevel::Loud:
		return TEXT("Loud");
	default:
		return TEXT("Unknown");
	}
}
