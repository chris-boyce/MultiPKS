// Fill out your copyright notice in the Description page of Project Settings.


#include "ScopeDisplay.h"
#include "UColorUtility.h"
#include "Components/TextBlock.h"
#include "Math/UnrealMathUtility.h"

float UScopeDisplay::MagnificationCalculation(float FOV)
{
	float OriginalFOVRadians = FMath::DegreesToRadians(90);
	float NewFOVRadians = FMath::DegreesToRadians(FOV);

	float OriginalTan = FMath::Tan(OriginalFOVRadians / 2.0f);
	float NewTan = FMath::Tan(NewFOVRadians / 2.0f);

	float Magnification = OriginalTan / NewTan;

	return Magnification;
}

void UScopeDisplay::SetAllText(FString Name, float ADSSpeed, float FOV, int AttachmentValue)
{
	TXT_ScopeName->SetText(FText::FromString(Name));
	
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.SetMaximumFractionalDigits(1);
	NumberFormatOptions.SetMinimumFractionalDigits(1);

	TXT_AttachmentValue->SetText(FText::AsNumber(AttachmentValue));
	
	FText FormattedText = FText::Format(NSLOCTEXT("YourContext", "YourKey", "{0}s"), FText::AsNumber(ADSSpeed, &NumberFormatOptions));
	TXT_ADSSpeed->SetText(FormattedText);
	
	FormattedText = FText::Format(NSLOCTEXT("YourContext", "YourKey", "{0}x"), FText::AsNumber(MagnificationCalculation(FOV), &NumberFormatOptions));
	TXT_ZoomAmount->SetText(FormattedText);
	UUColorUtility::SetBorderColorBasedOnValue(BDR_Color, AttachmentValue);
	
}


