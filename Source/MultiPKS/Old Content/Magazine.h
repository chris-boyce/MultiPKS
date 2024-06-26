// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"



UENUM(BlueprintType)
enum class EElementalType : uint8
{
	None UMETA(DisplayName="None"),
	Fire UMETA(DisplayName="Fire"),
	Ice UMETA(DisplayName="Ice"),
	Electric UMETA(DisplayName="Electric")
};

UENUM(BlueprintType)
enum class ERarity : uint8
{
	Blue UMETA(DisplayName="Common"),
	Purple UMETA(DisplayName="Uncommon"),
	Pink UMETA(DisplayName="Rare"),
	Red UMETA(DisplayName="Super"),
	Gold UMETA(DisplayName="Legendary")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReloadComplete, int, CurrentAmmo, int, MaxAmmo);

UCLASS()
class MULTIPKS_API AMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	AMagazine();

protected:
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditDefaultsOnly, Category="Mag")
	FString AttachmentName = "Magazine";
	
	UPROPERTY(EditDefaultsOnly, Category="Mag|Ammo")
	FIntPoint CurrentAmmoRange;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category="Mag|Ammo")
	int CurrentAmmo = 12;
	
	UPROPERTY(EditDefaultsOnly, Category="Mag|Ammo")
	FIntPoint MaxAmmoRange;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Mag|Ammo")
	int MaxAmmo = 12;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Reload")
	FVector2D ReloadSpeedRange;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Mag|Reload")
	float ReloadSpeed = 2.0f;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Mag|Elememtal")
	EElementalType BulletElementalType = EElementalType::None;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Elememtal")
	FVector2D ElementalPercentageChanceRange;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Mag|Elememtal")
	float ElementalPercentageChance = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Elememtal")
	FVector2D ElementalEffectTimeRange;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category="Mag|Elememtal")
	float ElementalEffectTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Rarity")
	ERarity Rarity =  ERarity::Blue;

	UPROPERTY(EditDefaultsOnly, Category="Seed")
	FString Seed = "1";

	UPROPERTY(Replicated, VisibleAnywhere, Category="Seed")
	int AttachmentScaleValue = 0;

	UFUNCTION()
	void AdjustScaleValue(int Scale);
	
	bool CanFire();

	bool isReloading = false;

	void HandleReloadComplete();

	void ConsumeAmmo();
	
	void ReloadMag();

	FOnReloadComplete OnReloadComplete;
	
	FTimerHandle ReloadTimeHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

};
