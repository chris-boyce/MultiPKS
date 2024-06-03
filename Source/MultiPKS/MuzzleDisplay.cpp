// Fill out your copyright notice in the Description page of Project Settings.


#include "MuzzleDisplay.h"
#include "Muzzle.h"
#include "Components/TextBlock.h"

void UMuzzleDisplay::SetAllText(FString MuzzleName, ESoundLevel SoundLevel, float BulletVelocity, int AttachmentValue)
{
	TXT_MuzzleName->SetText(FText::FromString(MuzzleName));
	TXT_SoundLevel->SetText(FText::FromString(GetSoundLevelString(SoundLevel)));
	TXT_BulletVelocity->SetText(FText::FromString(FString::Printf(TEXT("%.2fm/s"), BulletVelocity / 1000)));
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
