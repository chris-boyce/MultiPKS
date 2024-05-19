// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiPKSCharacter.h"
#include "Magazine.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "MultiPKS/Pickupable.h"
#include "MultiPKS/PlayerAmmoHUD.h"
#include "MultiPKS/WeaponDisplay.h"
#include "BasePistol.generated.h"

class ABarrel;
class AScope;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoUpdate);

UCLASS()
class MULTIPKS_API ABasePistol : public AActor , public IPickupable
{
	GENERATED_BODY()
	
public:	
	ABasePistol();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	
	/*---------- Weapon UI ----------*/

	void SetMagDisplay(AThirdPersonCharacter* InteractingCharacter);

	bool WeaponDisplayOnScreen = false;
	
	UPROPERTY()
	UUserWidget* WeaponDisplayWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UWeaponDisplay> WeaponDisplay;
	
	/*---------- Debug Editor ----------*/
	
	UPROPERTY(EditDefaultsOnly)
	bool EditMode = false;

	/*---------- Firing ----------*/

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void FireDown(AThirdPersonCharacter* Char);

	UFUNCTION()
	void FireUp(AThirdPersonCharacter* FiringCharacter);

	UFUNCTION()
	void Fire(AThirdPersonCharacter* Char);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	FTimerHandle FiringTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category=Projectile)
	TSubclassOf<AActor> ProjectileClass;
	
	
	/*---------- Picking Up ----------*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponentZ;
	
	UFUNCTION(BlueprintImplementableEvent)
	void EnableOutline();

	UFUNCTION(BlueprintImplementableEvent)
	void DisableOutline();

	virtual void HighlightObject(AThirdPersonCharacter* InteractingCharacter) override;

	UFUNCTION(BlueprintCallable)
	virtual void UnHighlightObject(AThirdPersonCharacter* InteractingCharacter) override;

	virtual ABasePistol* PickupObject(AThirdPersonCharacter* InteractingCharacter) override;
	

	/*---------- Effects ----------*/

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> FiringCameraShake;
	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_FireSound(FVector Location);

	FRotator OriginRotation;
	
	/*---------- Magazine ----------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<TSubclassOf<AMagazine>> MagazineClasses;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	AMagazine* MagazineComponent;
	
	FOnAmmoUpdate OnAmmoUpdate;

	/*---------- Scope ----------*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<TSubclassOf<AScope>> ScopeClasses;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	AScope* ScopeComponent;

	/*---------- Barrel ----------*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<TSubclassOf<ABarrel>> BarrelClasses;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	ABarrel* BarrelComponent;
	
};
