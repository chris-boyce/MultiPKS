// Fill out your copyright notice in the Description page of Project Settings.
#include "MagDisplay.h"
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

	FLinearColor NewColor;

	if (AttachmentScore >= 0 && AttachmentScore <= 10)
	{
		NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f); 
	}
	else if (AttachmentScore >= 11 && AttachmentScore <= 25)
	{
		NewColor = FLinearColor(1.0f, 0.41f, 0.71f, 0.8f); 
	}
	else if (AttachmentScore >= 26 && AttachmentScore <= 50)
	{
		NewColor = FLinearColor(0.5f, 0.0f, 0.5f, 0.8f);
	}
	else if (AttachmentScore >= 51 && AttachmentScore <= 75)
	{
		NewColor = FLinearColor(0.0f, 1.0f, 1.0f, 0.8f);
	}
	else if (AttachmentScore >= 76 && AttachmentScore <= 100)
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
