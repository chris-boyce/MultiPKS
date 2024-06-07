// Fill out your copyright notice in the Description page of Project Settings.


#include "GripDisplay.h"

#include "UColorUtility.h"
#include "Components/TextBlock.h"

void UGripDisplay::SetAllText(FString GripName, float UnADSMovementSpeed, float ADSMovementSpeed, float RecoilAmount, int AttachmentValue)
{
	TXT_GripName->SetText(FText::FromString(GripName));
	TXT_UnADSMovementSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.2fm/s"), UnADSMovementSpeed/100)));
	TXT_ADSedMovementSpeed->SetText(FText::FromString(FString::Printf(TEXT("%.2fm/s"), ADSMovementSpeed/100)));
	TXT_RecoilAmount->SetText(FText::FromString(FString::Printf(TEXT("%.2f°"), RecoilAmount)));
	TXT_AttachmentValue->SetText(FText::AsNumber(AttachmentValue));

	UUColorUtility::SetBorderColorBasedOnValue(BDR_Color, AttachmentValue);
}
