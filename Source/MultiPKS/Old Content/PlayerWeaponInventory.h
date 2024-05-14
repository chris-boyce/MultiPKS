// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerWeaponInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateInventory);

class ABasePistol;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPKS_API UPlayerWeaponInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerWeaponInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere)
	TArray<ABasePistol*> GunInventory;

	UFUNCTION()
	void AddWeaponToInventory(ABasePistol* Gun);

	FOnUpdateInventory OnUpdateInventory;
		
};