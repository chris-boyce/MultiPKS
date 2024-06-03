// Fill out your copyright notice in the Description page of Project Settings.


#include "BarrelDisplay.h"

#include "Components/TextBlock.h"
#include "Barrel.h"

void UBarrelDisplay::SetAllText(FString BarrelName, float FireRate, float Damage, EFireMode FireMode, float BurstSpeed,
                                int BurstCount, int ScaleValue)
{
	TXT_BarrelName->SetText(FText::FromString(BarrelName));

	TXT_FireRate->SetText(FText::FromString(FString::Printf(TEXT("%.2fs"), FireRate)));

	TXT_Damage->SetText(FText::FromString(FString::SanitizeFloat(Damage)));

	TXT_FireMode->SetText(FText::FromString(GetFireModeString(FireMode)));

	TXT_BurstSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.2fs"), BurstSpeed)));

	TXT_BurstCount->SetText(FText::FromString(FString::FromInt(BurstCount)));

	TXT_AttachmentValue->SetText(FText::AsNumber(ScaleValue));

	FLinearColor NewColor;

    if (ScaleValue >= 0 && ScaleValue <= 10)
    {
    	NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f); 
    }
    else if (ScaleValue >= 11 && ScaleValue <= 25)
    {
        NewColor = FLinearColor(1.0f, 0.41f, 0.71f, 0.8f); 
    }
    else if (ScaleValue >= 26 && ScaleValue <= 50)
    {
    	NewColor = FLinearColor(0.5f, 0.0f, 0.5f, 0.8f);
    }
    else if (ScaleValue >= 51 && ScaleValue <= 75)
    {
    	NewColor = FLinearColor(0.0f, 1.0f, 1.0f, 0.8f);
    }
    else if (ScaleValue >= 76 && ScaleValue <= 100)
    {
    	NewColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.8f);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Value is out of range"));
        return;
    }

    FSlateBrush Brush = BDR_Color->Background;
    Brush.TintColor = FSlateColor(NewColor);
    BDR_Color->SetBrush(Brush);
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
