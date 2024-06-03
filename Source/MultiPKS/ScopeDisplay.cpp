// Fill out your copyright notice in the Description page of Project Settings.


#include "ScopeDisplay.h"

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

	FLinearColor NewColor;

	if (AttachmentValue >= 0 && AttachmentValue <= 10)
	{
		NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f); 
	}
	else if (AttachmentValue >= 11 && AttachmentValue <= 25)
	{
		NewColor = FLinearColor(1.0f, 0.41f, 0.71f, 0.8f); 
	}
	else if (AttachmentValue >= 26 && AttachmentValue <= 50)
	{
		NewColor = FLinearColor(0.5f, 0.0f, 0.5f, 0.8f);
	}
	else if (AttachmentValue >= 51 && AttachmentValue <= 75)
	{
		NewColor = FLinearColor(0.0f, 1.0f, 1.0f, 0.8f);
	}
	else if (AttachmentValue >= 76 && AttachmentValue <= 100)
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


