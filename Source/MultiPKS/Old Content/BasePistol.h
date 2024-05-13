// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magazine.h"
#include "MultiPKSCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MultiPKS/Pickupable.h"
#include "BasePistol.generated.h"

UCLASS()
class MULTIPKS_API ABasePistol : public AActor , public IPickupable
{
	GENERATED_BODY()
	
public:	
	ABasePistol();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void HighlightObject() override;

	virtual void UnHighlightObject() override;

	virtual ABasePistol* PickupObject(AThirdPersonCharacter* InteractingCharacter) override;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category=Gameplay)
	TSubclassOf<AMagazine> MagazineClass;

	UPROPERTY(VisibleAnywhere)
	AMagazine* MagazineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponentZ;

	UPROPERTY()
	AMultiPKSCharacter* Character;

	UFUNCTION(BlueprintImplementableEvent)
	void EnableOutline();

	UFUNCTION(BlueprintImplementableEvent)
	void DisableOutline();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnFire(FVector Location, FRotator Rotation);
	bool Server_OnFire_Validate(FVector Location, FRotator Rotation);
	void Server_OnFire_Implementation(FVector Location, FRotator Rotation);
};
