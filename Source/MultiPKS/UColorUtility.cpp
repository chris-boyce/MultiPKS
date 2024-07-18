// Fill out your copyright notice in the Description page of Project Settings.


#include "UColorUtility.h"

#include "Components/Border.h"

void UUColorUtility::SetBorderColorBasedOnValue(UBorder* BorderComponent, int32 Value)
{
	if (!BorderComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BorderComponent is not valid"));
		return;
	}

	FLinearColor NewColor;

	if (Value >= 0 && Value <= 10)
	{
		NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f); // Red color with alpha 0.8
	}
	else if (Value >= 11 && Value <= 25)
	{
		NewColor = FLinearColor(1.0f, 0.41f, 0.71f, 0.8f); // Pink color with alpha 0.8
	}
	else if (Value >= 26 && Value <= 50)
	{
		NewColor = FLinearColor(0.5f, 0.0f, 0.5f, 0.8f); // Purple color with alpha 0.8
	}
	else if (Value >= 51 && Value <= 75)
	{
		NewColor = FLinearColor(0.0f, 1.0f, 1.0f, 0.8f); // Cyan color with alpha 0.8
	}
	else if (Value >= 76 && Value <= 100)
	{
		NewColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.8f); // Gray color with alpha 0.8
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Value is out of range"));
		return;
	}

	FSlateBrush Brush = BorderComponent->Background;
	Brush.TintColor = FSlateColor(NewColor);
	BorderComponent->SetBrush(Brush);
}

FLinearColor UUColorUtility::SetColorBasedOnValue(int32 Value)
{
	FLinearColor NewColor;

	if (Value >= 0 && Value <= 10)
	{
		NewColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f); // Red color with alpha 0.8
	}
	else if (Value >= 11 && Value <= 25)
	{
		NewColor = FLinearColor(1.0f, 0.41f, 0.71f, 0.8f); // Pink color with alpha 0.8
	}
	else if (Value >= 26 && Value <= 50)
	{
		NewColor = FLinearColor(0.5f, 0.0f, 0.5f, 0.8f); // Purple color with alpha 0.8
	}
	else if (Value >= 51 && Value <= 75)
	{
		NewColor = FLinearColor(0.0f, 1.0f, 1.0f, 0.8f); // Cyan color with alpha 0.8
	}
	else if (Value >= 76 && Value <= 100)
	{
		NewColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.8f); // Gray color with alpha 0.8
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Value is out of range"));
		NewColor =  FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);;
	}

	return NewColor;
}
