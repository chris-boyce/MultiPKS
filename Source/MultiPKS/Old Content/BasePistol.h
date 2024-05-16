// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Magazine.h"
#include "MultiPKSCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "MultiPKS/Pickupable.h"
#include "MultiPKS/PlayerAmmoHUD.h"
#include "MultiPKS/WeaponDisplay.h"
#include "BasePistol.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoUpdate, int, CurrentAmmo, int, MaxAmmo);

UCLASS()
class MULTIPKS_API ABasePistol : public AActor , public IPickupable
{
	GENERATED_BODY()
	
public:	
	ABasePistol();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void HighlightObject(AThirdPersonCharacter* InteractingCharacter) override;

	UFUNCTION(BlueprintCallable)
	virtual void UnHighlightObject(AThirdPersonCharacter* InteractingCharacter) override;

	virtual ABasePistol* PickupObject(AThirdPersonCharacter* InteractingCharacter) override;

	void SetMagDisplay();

	bool WeaponDisplayOnScreen = false;

	UPROPERTY()
	UUserWidget* WeaponDisplayWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UWeaponDisplay> WeaponDisplay;
	
	UPROPERTY(EditAnywhere, Category=Projectile)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireDown(AThirdPersonCharacter* Char);

	UFUNCTION()
	void FireUp();

	UFUNCTION()
	void Fire(AThirdPersonCharacter* Char);

	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category=Gameplay)
	TSubclassOf<AMagazine> MagazineClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<TSubclassOf<AMagazine>> MagazineClasses;

	UPROPERTY(EditDefaultsOnly)
	bool EditMode = false;

	FTimerHandle FiringTimerHandle;

	UPROPERTY(VisibleAnywhere)
	AMagazine* MagazineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponentZ;
	
	UFUNCTION(BlueprintImplementableEvent)
	void EnableOutline();

	UFUNCTION(BlueprintImplementableEvent)
	void DisableOutline();

	UFUNCTION()
	void BindAmmoToHUD(AThirdPersonCharacter* Character);
	UFUNCTION()
	void UnBindAmmoToHUD(AThirdPersonCharacter* Character);

	UPROPERTY()
	UPlayerAmmoHUD* PlayerAmmoHUD;

	FOnAmmoUpdate OnAmmoUpdate;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_FireSound(FVector Location);
	
};
