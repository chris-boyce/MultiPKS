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
	APlayerController* PC = Cast<APlayerController>(GetController());
	if(PlayerAmmoHUDClass && PC)
	{
		PlayerAmmoHUD = CreateWidget<UPlayerAmmoHUD>(PC, PlayerAmmoHUDClass);
	}
	
	MainCamera->SetActive(true);
	ADSCamera->SetActive(false);
	if(InteractComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Bound Drop"));
		InteractComponent->DropWeapon.AddDynamic(this, &AThirdPersonCharacter::HandleDropWeapon);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Has NOT Bound Drop"));
	}
	
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
	isArmed = true;
}

void AThirdPersonCharacter::HandleFireDown()
{
	if(!PlayerWeapon.IsEmpty())
	{
		if(HasAuthority())
		{
			PlayerWeapon[CurrentlySelectedWeapon]->Fire(this);
		}
		else
		{
			Server_Fire(PlayerWeapon[CurrentlySelectedWeapon]);
		}
		UpdateAmmoHUD(PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->CurrentAmmo, PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->MaxAmmo);
	
	}
	
}

void AThirdPersonCharacter::HandleFireUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Up"));
	if(!PlayerWeapon.IsEmpty())
	{
		PlayerWeapon[CurrentlySelectedWeapon]->FireUp();
	}
	
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

void AThirdPersonCharacter::HideWeapons()
{
	if(HasAuthority())
	{
		Multi_SwitchWeapon(this);
	}
	else
	{
		Server_SwitchWeapon(this);
	}
	PlayerWeapon[CurrentlySelectedWeapon]->BindAmmoToHUD(this);
}

void AThirdPersonCharacter::HandleDropWeapon(AThirdPersonCharacter* PlayerDropping)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Name: %s"), *PlayerDropping->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Recieved Broadcast"));
	
	PlayerWeapon[CurrentlySelectedWeapon]->UnBindAmmoToHUD(this);
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Authority"));
		Multi_DropWeapon(PlayerWeapon[CurrentlySelectedWeapon]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Doesnt Have Aothorirty"));
		Server_DropWeapon(PlayerWeapon[CurrentlySelectedWeapon]);
	}
}

void AThirdPersonCharacter::UpdateAmmoHUD(int CurrentAmmo, int MaxAmmo)
{
	PlayerAmmoHUD->SetAmmoText(CurrentAmmo, MaxAmmo);
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
		EnhancedInputComponent->BindAction(FireDownAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleFireDown);
		EnhancedInputComponent->BindAction(FireUpAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleFireUp);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleCrouch);

		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleADS);

		//EnhancedInputComponent->BindAction(DropWeaponAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleDropWeapon);

		EnhancedInputComponent->BindAction(FirstWeaponAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleFirstWeaponSwap);

		EnhancedInputComponent->BindAction(SecondWeaponAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleSecondWeaponSwap);
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

void AThirdPersonCharacter::Server_Fire_Implementation(ABasePistol* Gun)
{
	PlayerWeapon[CurrentlySelectedWeapon]->Fire(this);
}

bool AThirdPersonCharacter::Server_Fire_Validate(ABasePistol* Gun)
{
	return true;
}

void AThirdPersonCharacter::Server_ChangeSelectedWeapon_Implementation(int Num)
{
	Multi_ChangeSelectedWeapon(Num);
}

bool AThirdPersonCharacter::Server_ChangeSelectedWeapon_Validate(int Num)
{
	return true;
}

void AThirdPersonCharacter::Multi_ChangeSelectedWeapon_Implementation(int Num)
{
	CurrentlySelectedWeapon = Num;
}

bool AThirdPersonCharacter::Multi_ChangeSelectedWeapon_Validate(int Num)
{
	return true;
}

void AThirdPersonCharacter::SetCurrentSelectedWeapon(int Num)
{
	if(HasAuthority())
	{
		Multi_ChangeSelectedWeapon(Num);
	}
	else
	{
		Server_ChangeSelectedWeapon(Num);
	}
}

void AThirdPersonCharacter::Multi_SwitchWeapon_Implementation(AThirdPersonCharacter* Character)
{
	for (ABasePistol* Weapon : Character->PlayerWeapon)
	{
		if (Weapon)
		{
			Weapon->SetActorHiddenInGame(true);
		}
	}
	Character->PlayerWeapon[CurrentlySelectedWeapon]->SetActorHiddenInGame(false);
	
}

bool AThirdPersonCharacter::Multi_SwitchWeapon_Validate(AThirdPersonCharacter* Character)
{
	return true;
}

void AThirdPersonCharacter::Server_SwitchWeapon_Implementation(AThirdPersonCharacter* Character)
{
	Multi_SwitchWeapon(Character);
}

bool AThirdPersonCharacter::Server_SwitchWeapon_Validate(AThirdPersonCharacter* Character)
{
	return true;
}

void AThirdPersonCharacter::Multi_DropWeapon_Implementation(ABasePistol* DropWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Fired Multi"));
	if (!DropWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Gun is null."));
		return;
	}
	DropWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	DropWeapon->SphereComponentZ->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

bool AThirdPersonCharacter::Multi_DropWeapon_Validate(ABasePistol* DropWeapon)
{
	return true;
}

void AThirdPersonCharacter::Server_DropWeapon_Implementation(ABasePistol* DropWeapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Fired Server"));
	Multi_DropWeapon(DropWeapon);
}

bool AThirdPersonCharacter::Server_DropWeapon_Validate(ABasePistol* DropWeapon)
{
	return true;
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



