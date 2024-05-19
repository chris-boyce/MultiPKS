// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelDisplay.h"

#include "Components/TextBlock.h"
#include "Barrel.h"

void UBarrelDisplay::SetAllText(FString BarrelName, float FireRate, float Damage, EFireMode FireMode, float BurstSpeed,
                                int BurstCount)
{
	TXT_BarrelName->SetText(FText::FromString(BarrelName));

	TXT_FireRate->SetText(FText::FromString(FString::Printf(TEXT("%.2fs"), FireRate)));

	TXT_Damage->SetText(FText::FromString(FString::SanitizeFloat(Damage)));

	TXT_FireMode->SetText(FText::FromString(GetFireModeString(FireMode)));

	TXT_BurstSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.2fs"), BurstSpeed)));

	TXT_BurstCount->SetText(FText::FromString(FString::FromInt(BurstCount)));
}

FString UBarrelDisplay::GetFireModeString(EFireMode FireMode) const
{
	switch (FireMode)
	{
	case EFireMode::Semi:
		return TEXT("Single");
	case EFireMode::Burst:
		return TEXT("Burst");
	case EFireMode::Automatic:
		return TEXT("Automatic");
	default:
		return TEXT("Unknown");
	}
}
