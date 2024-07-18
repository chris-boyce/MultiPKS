// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "HealthBarDisplay.h"
#include "InGameSettingDisplay.h"
#include "NiagaraComponent.h"
#include "PlayerAmmoHUD.h"
#include "SettingsUtility.h"
#include "GameFramework/Character.h"
#include "Old Content/BasePistol.h"
#include "ThirdPersonCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UInteractComp;
class UArrowComponent;

UENUM(BlueprintType)  
enum class EWeaponType : uint8
{
	Rifle,
	Pistol,
	None 
};

UCLASS()
class MULTIPKS_API AThirdPersonCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	AThirdPersonCharacter();

protected:
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction*  FireDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction*  FireUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction*  CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ADSAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FirstWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SecondWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlinkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrapplingAction;
	
	USettingsUtility* SettingsUtility;

	/* ---- Slide Vars ----- */
	float SlideSpeedMultiplier = 2.5f;
	float SlideFriction = 0.1f;
	float SlideDuration = 1.0f;
	float MinSlideSpeed = 450.f;
	float MinForwardVelocityToSlide = 300.0f;
	float OriginalHeight;

	UPROPERTY(EditAnywhere, Category = "Teleport Dash")
	float DashDistance = 2500.0f;  // Distance of the dash in Unreal units

	UPROPERTY(EditAnywhere, Category = "Teleport Dash")
	float DashCooldown = 5.0f;  // Cooldown in seconds

	float HookRange = 2000.f; // 1000 Unreal units
	float HookSpeed = 1500.f;
	
	bool bCanDash = true;
	
	FVector GrapplingHitLocation;

	float OriginalFriction;
	
	FTimerHandle RespawnTimer;


	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void HandleInteract();

	void HandleFireDown();

	void HandleFireUp();

	void HandleCrouch();

	void HandleADS();

	void HandleReload();

	void HandleFirstWeaponSwap();

	void HandleSecondWeaponSwap();

	void HandleOpenMenu();

	bool isMenuOpen = false;

	bool CanPerformAction = true;

	void HideWeapons();

	/* ---- Server-Client Slide ---- */

	void HandleSlide();

	void StopSlide();

	UFUNCTION(Server, Reliable)
	void Server_Slide();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Slide();

	



	/* ---- Server-Client Blink ---- */
	void HandleBlink();

	void ResetDash();
	
	UFUNCTION(Server, Reliable)
	void Server_Blink();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Blink();


	/* ---- Server-Client Grappling ---- */
	void HandleGrappling();

	void PullPlayerToHook();

	void RestoreFriction();
	
	UFUNCTION(Server, Reliable)
	void Server_Grappling();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Grappling();
	
	UNiagaraComponent* FindNiagaraComponentByTag(const FName& Tag);

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Particle")
	UNiagaraComponent* DashParticleSystem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Particle")
	UNiagaraComponent* BeamNiagaraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* BeamTemplate;



	
	
	USkeletalMeshComponent* GetPlayerMesh() const { return ThirdPersonPlayerMesh; }

	UPlayerAmmoHUD* GetPlayerAmmoHUD() const {return PlayerAmmoHUD;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	FVector EditableVector;

	void UpdateAmmoHUD(int CurrentAmmo, int MaxAmmo);

	UCameraComponent* GetMainCameraComponent() const { return MainCamera; }
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isCrouched = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isArmed = false;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool isADSed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isSliding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isDash = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isGrappling = false;

	UFUNCTION()
	bool GetIsADS() {return isADSed;}

	UFUNCTION(Server, Reliable)
	void SetADS(bool ADS);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|Mesh")
	USkeletalMeshComponent* ThirdPersonPlayerMesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|Cameras")
	UCameraComponent* MainCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|InteractComp")
	UInteractComp* InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|Widgets")
	TSubclassOf<UPlayerAmmoHUD> PlayerAmmoHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|Widgets")
	UPlayerAmmoHUD* PlayerAmmoHUD;

	UPROPERTY(VisibleAnywhere)
	TArray<ABasePistol*> PlayerWeapon;

	UPROPERTY()
	int CurrentlySelectedWeapon = 0;

	UFUNCTION()
	void HandleDropWeapon(AThirdPersonCharacter* PlayerDropping);
	
	UFUNCTION()
	void ChangeMoveSpeed(float MoveSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(AThirdPersonCharacter* ThisPlayer, ABasePistol* Gun);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_FireStop(ABasePistol* Gun);

	UFUNCTION(Server, Reliable)
	void Server_CallSetMag(ABasePistol* Gun);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveSpeed(float Speed);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_MoveSpeed(float Speed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DropWeapon(ABasePistol* DropWeapon);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_DropWeapon(ABasePistol* DropWeapon);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SwitchWeapon(AThirdPersonCharacter* Character);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_SwitchWeapon(AThirdPersonCharacter* Character);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ChangeSelectedWeapon(int Num);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ChangeSelectedWeapon(int Num);

	UFUNCTION()
	void SetCurrentSelectedWeapon(int Num);

	UFUNCTION(Client, Reliable)
	void Client_CallUpdateAmmo(int CurrentAmmo, int MaxAmmo);

	UFUNCTION(Client, Reliable)
	void Client_CallUpdateHealth(float UpdatedCurrentHealth, float UpdatedMaxHealth);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateHealth(float NewHealth);


	UFUNCTION()
	void ResetRotateCamera(float ResetTime);
	UFUNCTION(Server, Reliable)
	void Server_ResetRotateCamera(float ResetTime);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ResetRotateCamera(float ResetTime);

	
	UFUNCTION()
	void RotateCamera(float RotX, float RotY);
	UFUNCTION(Server, Reliable)
	void Server_RotateCamera(float RotX, float RotY);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_RotateCamera(float RotX, float RotY);

	UPROPERTY()
	FRotator OriginCameraRotation;

	FTimerHandle CameraResetTimerHandle;

	UFUNCTION(Server, Reliable)
	void Server_Reload();

	UFUNCTION(Client, Reliable)
	void Client_ScreenShake(TSubclassOf<UCameraShakeBase> Shake);

	UFUNCTION(Server, Reliable)
	void Server_ToggleCameraPosition(UCameraComponent* Camera, bool ADS);

	virtual void TakeDamage(float DamageAmount) override;

	virtual void DetailedTakeDamage(float DamageAmount, FVector HitLocation) override;

	UFUNCTION(BlueprintCallable)
	virtual void DetailedTakeDamage2(float DamageAmount, FVector HitLocation, FName BoneName) override;

	UFUNCTION()
	void Respawn();
	

	virtual float GetHealth() const override;

	UPROPERTY(VisibleAnywhere, Category="Health")
	float CurrentHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category="Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UHealthBarDisplay> HealthBarClass;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="UI")
	UHealthBarDisplay* HealthBarDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UInGameSettingDisplay> InGameSettingClass;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category="UI")
	UInGameSettingDisplay* InGameSettingDisplay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float LookSensitivity = 1.0f;

	UPROPERTY()
	float CurrentLookSensitivity = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetLookSensitivity(float NewSensitivity);
	
	

	void GetAllAttachedActorsRecursively(AActor* ParentActor, TArray<AActor*>& OutActors);
	
};
