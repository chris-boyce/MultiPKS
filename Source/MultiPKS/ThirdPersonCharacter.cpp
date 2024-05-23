// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "EnhancedInputComponent.h"
#include "Grip.h"
#include "Scope.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Old Content/InteractComp.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

AThirdPersonCharacter::AThirdPersonCharacter()
{
	ThirdPersonPlayerMesh = GetComponentByClass<USkeletalMeshComponent>();
}

void AThirdPersonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AThirdPersonCharacter, MainCamera);
	DOREPLIFETIME(AThirdPersonCharacter, isADSed);
}


void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = Cast<APlayerController>(GetController());
	if(PlayerAmmoHUDClass && PC)
	{
		PlayerAmmoHUD = CreateWidget<UPlayerAmmoHUD>(PC, PlayerAmmoHUDClass);
	}
	if(InteractComponent)
	{
		InteractComponent->DropWeapon.AddDynamic(this, &AThirdPersonCharacter::HandleDropWeapon);
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
			PlayerWeapon[CurrentlySelectedWeapon]->FireDown(this);
		}
		else
		{
			Server_Fire(this, PlayerWeapon[CurrentlySelectedWeapon]);
		}
		
		GetWorld()->GetTimerManager().ClearTimer(CameraResetTimerHandle);
		
	}
	
}

void AThirdPersonCharacter::HandleFireUp()
{
	if(!PlayerWeapon.IsEmpty())
	{
		if(HasAuthority())
		{
			PlayerWeapon[CurrentlySelectedWeapon]->FireUp(this);
		}
		else
		{
			Server_FireStop(PlayerWeapon[CurrentlySelectedWeapon]);
		}
	
		
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
	if(PlayerWeapon.Num() <= 0)
	{
		return;
	}
	
	if(isADSed)
	{
		PlayerWeapon[CurrentlySelectedWeapon]->GripComponent->ChangePlayerSpeed(this, isADSed);
		if(HasAuthority())
		{
			Server_ToggleCameraPosition(MainCamera, isADSed);
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleCameraPosition(MainCamera, isADSed); /* Please Leave In This Order */
			isADSed = false;
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleScopeVisual(this, isADSed);
		}
		else
		{
			Server_ToggleCameraPosition(MainCamera, isADSed);
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleCameraPosition(MainCamera, isADSed);
			SetADS(false);
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleScopeVisual(this, !isADSed);
		}
	}
	else
	{
		PlayerWeapon[CurrentlySelectedWeapon]->GripComponent->ChangePlayerSpeed(this, isADSed);
		if(HasAuthority())
		{
			Server_ToggleCameraPosition(MainCamera, isADSed);
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleCameraPosition(MainCamera, isADSed); /* Please Leave In This Order */
			isADSed = true;
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleScopeVisual(this, isADSed);
		}
		else
		{
			Server_ToggleCameraPosition(MainCamera, isADSed); /* Please Leave In This Order */
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleCameraPosition(MainCamera, isADSed);
			SetADS(true);
			PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleScopeVisual(this, !isADSed);
		}
		
	}
	
}

void AThirdPersonCharacter::HandleReload()
{
	if(PlayerWeapon.Num() <= 0)
	{
		return;
	}
	if(HasAuthority())
	{
		PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->ReloadMag();
		PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->OnReloadComplete.AddDynamic(this, &AThirdPersonCharacter::Client_CallUpdateAmmo);
	}
	else
	{
		Server_Reload();
	}
	
	
}

void AThirdPersonCharacter::HandleFirstWeaponSwap()
{
	if(isADSed)
	{
		return;
	}
	if(PlayerWeapon.Num() >= 1)
	{
		SetCurrentSelectedWeapon(0); HideWeapons();
	}
}

void AThirdPersonCharacter::HandleSecondWeaponSwap()
{
	if(isADSed)
	{
		return;
	}
	if(PlayerWeapon.Num() >= 2)
	{
		SetCurrentSelectedWeapon(1); HideWeapons();
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
}

void AThirdPersonCharacter::HandleDropWeapon(AThirdPersonCharacter* PlayerDropping)
{
	UE_LOG(LogTemp, Warning, TEXT("Character Name: %s"), *PlayerDropping->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Recieved Broadcast"));
	
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
	if(!HasAuthority())
	{
		PlayerAmmoHUD->SetAmmoText(CurrentAmmo, MaxAmmo);
	}
	else
	{
		PlayerAmmoHUD->SetAmmoText(CurrentAmmo, MaxAmmo);
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
		EnhancedInputComponent->BindAction(FireDownAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleFireDown);
		EnhancedInputComponent->BindAction(FireUpAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleFireUp);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleCrouch);

		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleADS);

		//EnhancedInputComponent->BindAction(DropWeaponAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleDropWeapon);

		EnhancedInputComponent->BindAction(FirstWeaponAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleFirstWeaponSwap);

		EnhancedInputComponent->BindAction(SecondWeaponAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleSecondWeaponSwap);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::HandleReload);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AThirdPersonCharacter::SetADS_Implementation(bool ADS)
{
	isADSed = ADS;
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

void AThirdPersonCharacter::Server_CallSetMag_Implementation(ABasePistol* Gun)
{
	Gun->SetMagDisplay(this);
}

void AThirdPersonCharacter::Server_FireStop_Implementation(ABasePistol* Gun)
{
	Gun->FireUp(this);
}

bool AThirdPersonCharacter::Server_FireStop_Validate(ABasePistol* Gun)
{
	return true;
}

void AThirdPersonCharacter::Server_Fire_Implementation(AThirdPersonCharacter* ThisPlayer, ABasePistol* Gun)
{
	PlayerWeapon[CurrentlySelectedWeapon]->FireDown(this);
}

bool AThirdPersonCharacter::Server_Fire_Validate(AThirdPersonCharacter* ThisPlayer, ABasePistol* Gun)
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
	Server_CallSetMag(PlayerWeapon[CurrentlySelectedWeapon]);
}

void AThirdPersonCharacter::ResetRotateCamera(float ResetTime)
{
	if(HasAuthority())
	{
		Multi_ResetRotateCamera(ResetTime);
	}
	else
	{
		Server_ResetRotateCamera(ResetTime);
	}
}

void AThirdPersonCharacter::Multi_ResetRotateCamera_Implementation(float ResetTime)
{
	GetWorld()->GetTimerManager().ClearTimer(CameraResetTimerHandle);
	
	FQuat InitialQuat = FRotator::ZeroRotator.Quaternion();
	FQuat CurrentQuat = MainCamera->GetRelativeRotation().Quaternion();
    
	float StartTime = GetWorld()->GetTimeSeconds(); 
    
	GetWorld()->GetTimerManager().SetTimer(CameraResetTimerHandle, [this, InitialQuat, CurrentQuat, ResetTime, StartTime]() mutable 
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float ElapsedTime = CurrentTime - StartTime;
        
		float Alpha = FMath::Clamp(ElapsedTime / ResetTime, 0.0f, 1.0f);
        
		FQuat NewQuat = FQuat::Slerp(CurrentQuat, InitialQuat, Alpha);
		MainCamera->SetRelativeRotation(NewQuat.Rotator());
        
		if (ElapsedTime >= ResetTime)
		{
			GetWorld()->GetTimerManager().ClearTimer(CameraResetTimerHandle);
		}
	}, 0.01f, true);
}

void AThirdPersonCharacter::RotateCamera(float RotX, float RotY)
{
	if(HasAuthority())
	{
		Multi_RotateCamera(RotX, RotY);
	}
	else
	{
		Server_RotateCamera(RotX,RotY);
	}
}

void AThirdPersonCharacter::GetAllAttachedActorsRecursively(AActor* ParentActor, TArray<AActor*>& OutActors)
{
	TArray<AActor*> DirectlyAttachedActors;
	ParentActor->GetAttachedActors(DirectlyAttachedActors);

	for (AActor* Actor : DirectlyAttachedActors)
	{
		if (!OutActors.Contains(Actor))
		{
			OutActors.Add(Actor); 
			Actor->SetActorHiddenInGame(false); 
			
			GetAllAttachedActorsRecursively(Actor, OutActors);
		}
	}
}

void AThirdPersonCharacter::Server_ToggleCameraPosition_Implementation(UCameraComponent* Camera, bool ADS)
{
	PlayerWeapon[CurrentlySelectedWeapon]->ScopeComponent->ToggleCameraPosition(Camera, ADS);
	
}

void AThirdPersonCharacter::Multi_RotateCamera_Implementation(float RotX, float RotY)
{
	FRotator rotation = MainCamera->GetComponentRotation();
	FRotator temp = FRotator(rotation.Pitch += RotX, rotation.Yaw += RotY, rotation.Roll);
	MainCamera->SetWorldRotation(temp);
}

void AThirdPersonCharacter::Server_Reload_Implementation()
{
	PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->ReloadMag();
	PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->OnReloadComplete.AddDynamic(this, &AThirdPersonCharacter::Client_CallUpdateAmmo);
}

void AThirdPersonCharacter::Server_ResetRotateCamera_Implementation(float ResetTime)
{
	Multi_ResetRotateCamera(ResetTime);
}

void AThirdPersonCharacter::Server_RotateCamera_Implementation(float RotX, float RotY)
{
	Multi_RotateCamera(RotX, RotY);
}

void AThirdPersonCharacter::Client_ScreenShake_Implementation(TSubclassOf<UCameraShakeBase> Shake)
{
	auto temp = Cast<APlayerController>(GetController());
	temp->ClientStartCameraShake(Shake, 10);
}

void AThirdPersonCharacter::Client_CallUpdateAmmo_Implementation(int CurrentAmmo, int MaxAmmo)
{
	UpdateAmmoHUD(CurrentAmmo ,MaxAmmo );
}


void AThirdPersonCharacter::Multi_SwitchWeapon_Implementation(AThirdPersonCharacter* Character)
{
	Character->PlayerWeapon[0]->SetActorHiddenInGame(true);
	Character->PlayerWeapon[1]->SetActorHiddenInGame(true);
	for (ABasePistol* Weapon : Character->PlayerWeapon)
	{
		TArray<AActor*> AttachedActors;
		GetAllAttachedActorsRecursively(Weapon, AttachedActors);
		for (AActor* Actor : AttachedActors)
		{
			Actor->SetActorHiddenInGame(true);
		}
	}
	TArray<AActor*> AttachedActors;
	Character->PlayerWeapon[CurrentlySelectedWeapon]->SetActorHiddenInGame(false);
	GetAllAttachedActorsRecursively(PlayerWeapon[CurrentlySelectedWeapon], AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		Actor->SetActorHiddenInGame(false);
	}
	
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
	if (!DropWeapon)
	{
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



