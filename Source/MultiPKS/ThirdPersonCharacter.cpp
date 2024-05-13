// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Old Content/InteractComp.h"
#include "GameFramework/CharacterMovementComponent.h"

AThirdPersonCharacter::AThirdPersonCharacter()
{
	ThirdPersonPlayerMesh = GetComponentByClass<USkeletalMeshComponent>();
}

void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	MainCamera->SetActive(true);
	ADSCamera->SetActive(false);
}

void AThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void AThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AThirdPersonCharacter::HandleInteract()
{
	InteractComponent->InteractWithObject();
}

void AThirdPersonCharacter::HandleFire()
{
	UE_LOG(LogTemp, Log, TEXT("Fire"));
}

void AThirdPersonCharacter::HandleCrouch()
{
	if(isCrouched)
	{
		UnCrouch();
		isCrouched = false;
	}
	else
	{
		Crouch();
		isCrouched = true;
	}
}

void AThirdPersonCharacter::HandleADS()
{
	UCharacterMovementComponent* CharMovementComp = GetCharacterMovement();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(isADSed)
	{
		ChangeMoveSpeed(500.0f);
		MainCamera->SetActive(true);
		ADSCamera->SetActive(false);
		isADSed = false;
	}
	else
	{
		ChangeMoveSpeed(200.0f);
		ADSCamera->SetActive(true);
		MainCamera->SetActive(false);
		isADSed = true;
	}
}

void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Look);
		
		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleInteract);

		//Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleFire);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleCrouch);

		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleADS);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AThirdPersonCharacter::ChangeMoveSpeed(float MoveSpeed)
{
	if(HasAuthority())
	{
		Multi_MoveSpeed(MoveSpeed);
	}
	else
	{
		Server_MoveSpeed(MoveSpeed);
	}
}

void AThirdPersonCharacter::Multi_MoveSpeed_Implementation(float Speed)
{
	UCharacterMovementComponent* CharMovementComp = GetCharacterMovement();
	CharMovementComp->MaxWalkSpeed = Speed;
}

bool AThirdPersonCharacter::Multi_MoveSpeed_Validate(float Speed)
{
	return true;
}

void AThirdPersonCharacter::Server_MoveSpeed_Implementation(float Speed)
{
	Multi_MoveSpeed(Speed);
}

bool AThirdPersonCharacter::Server_MoveSpeed_Validate(float Speed)
{
	return true;
}



