// Fill out your copyright notice in the Description page of Project Settings.


#include "GripDisplay.h"

#include "Components/TextBlock.h"

void UGripDisplay::SetAllText(FString GripName, float UnADSMovementSpeed, float ADSMovementSpeed, float RecoilAmount, int AttachmentValue)
{
	TXT_GripName->SetText(FText::FromString(GripName));
	TXT_UnADSMovementSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.2fm/s"), UnADSMovementSpeed/100)));
	TXT_ADSedMovementSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.2fm/s"), ADSMovementSpeed/100)));
	TXT_RecoilAmount->SetText(FText::FromString(FString::Printf(TEXT("%.2f°"), RecoilAmount)));
	TXT_AttachmentValue->SetText(FText::AsNumber(AttachmentValue));

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
