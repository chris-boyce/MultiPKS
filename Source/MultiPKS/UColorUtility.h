// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UColorUtility.generated.h"

class UBorder;
UCLASS()
class MULTIPKS_API UUColorUtility : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	static void SetBorderColorBasedOnValue(UBorder* BorderComponent, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "UI")
	static FLinearColor SetColorBasedOnValue(int32 Value);
	
};
