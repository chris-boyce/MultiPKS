// Fill out your copyright notice in the Description page of Project Settings.
#include "MagDisplay.h"

#include "UColorUtility.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"


void UMagDisplay::SetAllText(FString Name, int Capacity, float ReloadSpeed, FString Effect,
	float EffectChance, float EffectDuration, int AttachmentScore)
{
	UE_LOG(LogTemp, Warning, TEXT("Run SetAllText"));
	//RunCompSetup();
	TXT_MagName->SetText(FText::FromString(Name));
	
	TXT_Capacity->SetText(FText::AsNumber(Capacity));

	TXT_AttachmentScore->SetText(FText::AsNumber(AttachmentScore));
	
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

	UUColorUtility::SetBorderColorBasedOnValue(BDR_Color, AttachmentScore);
	
}
