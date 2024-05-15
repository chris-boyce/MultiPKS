// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "MagDisplay.h"

void UMagDisplay::SetAllText(FString Name, FString Rarity, int Capacity, float ReloadSpeed, FString Effect,
	float EffectChance, float EffectDuration)
{
	UE_LOG(LogTemp, Warning, TEXT("Run SetAllText"));
	//RunCompSetup();
	TXT_MagName->SetText(FText::FromString(Name));
	
	TXT_Rarity->SetText(FText::FromString(Rarity));
	TXT_Capacity->SetText(FText::AsNumber(Capacity));
	
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.SetMaximumFractionalDigits(1);
	NumberFormatOptions.SetMinimumFractionalDigits(1);
	FText FormattedText = FText::Format(NSLOCTEXT("YourContext", "YourKey", "{0}s"), FText::AsNumber(ReloadSpeed, &NumberFormatOptions));
	TXT_ReloadSpeed->SetText((FormattedText));

	TXT_Effect->SetText(FText::FromString(Effect));
	
	FormattedText = FText::Format(NSLOCTEXT("YourContext", "YourKey", "{0}%"), FText::AsNumber(EffectChance, &NumberFormatOptions));
	TXT_EffectChance->SetText(FormattedText);
	
	FormattedText = FText::Format(NSLOCTEXT("YourContext", "YourKey", "{0}s"), FText::AsNumber(EffectDuration, &NumberFormatOptions));
	TXT_ElementDuration->SetText(FormattedText);
	
}
