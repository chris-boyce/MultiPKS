// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiPKSCharacter.h"
#include "Magazine.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "MultiPKS/GunDataSingleton.h"
#include "MultiPKS/Pickupable.h"
#include "MultiPKS/PlayerAmmoHUD.h"
#include "MultiPKS/WeaponDisplay.h"
#include "BasePistol.generated.h"

class AMuzzle;
class ABarrel;
class AScope;
class AGrip;
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
	UWeaponDisplay* WeaponDisplayWidget;
	
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
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	AMagazine* MagazineComponent;
	
	FOnAmmoUpdate OnAmmoUpdate;

	/*---------- Scope ----------*/
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	AScope* ScopeComponent;

	/*---------- Barrel ----------*/
	

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	ABarrel* BarrelComponent;

	/*---------- Muzzle ----------*/
	

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	AMuzzle* MuzzleComponent;

	/*---------- Grip ----------*/
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	AGrip* GripComponent;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MainMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Seed")
	bool GunIsRandom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Seed")
	FString GunSeed;

	UPROPERTY(VisibleAnywhere, Category="Seed");
	FString GunData;

private:
	UPROPERTY(EditAnywhere, Category="Singleton", meta=(AllowPrivateAccess = "true"))
	TSubclassOf<UGunDataSingleton> GunDataSingletonClass;

	UPROPERTY(VisibleAnywhere, Category="Singleton")
	UGunDataSingleton* GunDataSingleton;

	UPROPERTY(Replicated, VisibleAnywhere)
	EWeaponTypes WeaponType;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	FReturnWeaponData WeaponData;

	FString GenerateRandomSeed();

	FString ExtractGunDataFromSeed(const FString& OriginalString);

	void ExtractValueDataFromSeed(const FString& OriginalString);

	UPROPERTY(VisibleAnywhere, Category = "Seed")
	TArray<int> ValueData;

	UPROPERTY(VisibleAnywhere, Category = "Seed")
	TArray<int> TransformedValueData;

	int ExponentialValueShift(int GunBaseValue, int AttachmentBaseValue);


	
	
};
