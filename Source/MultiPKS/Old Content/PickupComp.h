// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupComp.generated.h"


class AMultiPKSCharacter;
class UBoxComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPKS_API UPickupComp : public UActorComponent
{
	GENERATED_BODY()
public:	
	UPickupComp();
protected:
	virtual void BeginPlay() override;
public:

	UFUNCTION()
	void AttachGun(AMultiPKSCharacter* Character);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AttachGun(AMultiPKSCharacter* InteractingCharacter);
	bool Server_AttachGun_Validate(AMultiPKSCharacter* InteractingCharacter);
	void Server_AttachGun_Implementation(AMultiPKSCharacter* InteractingCharacter);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_AttachGun(AMultiPKSCharacter* InteractingCharacter);
	bool Multi_AttachGun_Validate(AMultiPKSCharacter* InteractingCharacter);
	void Multi_AttachGun_Implementation(AMultiPKSCharacter* InteractingCharacter);
		
};
