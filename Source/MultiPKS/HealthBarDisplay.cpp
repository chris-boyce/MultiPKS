// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarDisplay.h"

#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBarDisplay::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (MaxHealth <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("MaxHealth is less than or equal to 0."));
		return;
	}
	if(CurrentHealth <=0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("YYYYYYYYYYY"));
		return;
	}

	float HealthPercentage = CurrentHealth / MaxHealth;
	HealthPercentage = FMath::Clamp(HealthPercentage, 0.0f, 1.0f);
	
	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %f"), CurrentHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health Percentage: %f"), HealthPercentage);
	
	float InverseHealthPercentage = 1.0f - HealthPercentage;
	UE_LOG(LogTemp, Warning, TEXT("Inverse Health Percentage: %f"), InverseHealthPercentage);
	
	if (InverseHealthPercentage >= 0.65f)
	{
		TargetOpacity = InverseHealthPercentage;
	}
	
	int HealthPercentageInt = FMath::RoundToInt(HealthPercentage * 100.0f);
	FText HealthFText = FText::FromString(FString::Printf(TEXT("%d%%"), HealthPercentageInt));

	HealthTextZ->SetText(HealthFText);

}

void UHealthBarDisplay::UpdateOpacity()
{
	if (CurrentOpacity < TargetOpacity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Running Loop"));
		CurrentOpacity = FMath::Min(CurrentOpacity + FadeSpeed * 0.01f, TargetOpacity);
		BloodBorderZ->SetBrushColor(FLinearColor(1.0f, 1.0f, 1.0f, CurrentOpacity));

		if (CurrentOpacity >= TargetOpacity)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loop Stopped"));
			GetWorld()->GetTimerManager().ClearTimer(OpacityTimerHandle);
		}
	}
}

void UHealthBarDisplay::NativeConstruct()
{
	Super::NativeConstruct();
	
}


