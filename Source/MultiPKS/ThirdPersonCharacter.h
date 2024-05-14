// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Old Content/BasePistol.h"
#include "ThirdPersonCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UInteractComp;

UENUM(BlueprintType)  
enum class EWeaponType : uint8
{
	Rifle,
	Pistol,
	None 
};

UCLASS()
class MULTIPKS_API AThirdPersonCharacter : public ACharacter
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
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void HandleInteract();

	void HandleFireDown();

	void HandleFireUp();

	void HandleCrouch();

	void HandleADS();

	void HandleFirstWeaponSwap(){SetCurrentSelectedWeapon(0); UE_LOG(LogTemp, Warning, TEXT("Currently Gun 0")); HideWeapons();}

	void HandleSecondWeaponSwap(){SetCurrentSelectedWeapon(1);UE_LOG(LogTemp, Warning, TEXT("Currently Gun 1")); HideWeapons();}

	void HideWeapons();

public:
	USkeletalMeshComponent* GetPlayerMesh() const { return ThirdPersonPlayerMesh; }

	UCameraComponent* GetMainCameraComponent() const { return MainCamera; }
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isCrouched = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isArmed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isADSed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|Mesh")
	USkeletalMeshComponent* ThirdPersonPlayerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|Cameras")
	UCameraComponent* MainCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|Cameras")
	UCameraComponent* ADSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ThirdPersonPlayer|InteractComp")
	UInteractComp* InteractComponent;

	UPROPERTY(VisibleAnywhere)
	TArray<ABasePistol*> PlayerWeapon;

	UPROPERTY()
	int CurrentlySelectedWeapon = 0;

	UFUNCTION()
	void HandleDropWeapon(AThirdPersonCharacter* PlayerDropping);
	
	UFUNCTION()
	void ChangeMoveSpeed(float MoveSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Fire(ABasePistol* Gun);

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

	
};
