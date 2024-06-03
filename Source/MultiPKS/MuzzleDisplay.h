// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "MuzzleDisplay.generated.h"

enum class ESoundLevel : uint8;
class UTextBlock;


UCLASS()
class MULTIPKS_API UMuzzleDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_MuzzleName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_SoundLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_BulletVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UTextBlock* TXT_AttachmentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetText", meta = (BindWidget))
	UBorder* BDR_Color;

	UFUNCTION()
	void SetAllText(FString MuzzleName, ESoundLevel SoundLevel, float BulletVelocity, int AttachmentValue);

	FString GetSoundLevelString(ESoundLevel SoundLevel) const;
};
