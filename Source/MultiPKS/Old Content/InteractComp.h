// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePistol.h"
#include "Components/ActorComponent.h"
#include "InteractComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDropWeapon, AThirdPersonCharacter*, Player);

class IPickupable;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPKS_API UInteractComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractComp();

	virtual void InteractWithObject();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AttachGun(ABasePistol* Gun);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_AttachGun(ABasePistol* Gun);
	
	FDropWeapon DropWeapon;

protected:
	
	void PerformLineTrace();
	
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle;
	
	
	IPickupable* SavedPickupable;
	
};
