// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseMagazine.generated.h"

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


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPKS_API UBaseMagazine : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBaseMagazine();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Ammo")
	int CurrentAmmo = 12;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Ammo")
	int MaxAmmo = 12;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Reload")
	float ReloadSpeed = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Elememtal")
	EElementalType BulletElementalType = EElementalType::None;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Elememtal")
	float ElementalPercentageChance = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Elememtal")
	float ElementalEffectTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category="Mag|Rarity")
	ERarity Rarity =  ERarity::Blue;

		
};
