// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UCameraComponent;
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
	UInputAction*  FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction*  CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ADSAction;
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void HandleInteract();

	void HandleFire();

	void HandleCrouch();

	void HandleADS();

	

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
};
