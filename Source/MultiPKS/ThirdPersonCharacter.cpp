// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
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
	DOREPLIFETIME(AThirdPersonCharacter, ADSCamera);
	
}


void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = Cast<APlayerController>(GetController());
	if(PlayerAmmoHUDClass && PC)
	{
		PlayerAmmoHUD = CreateWidget<UPlayerAmmoHUD>(PC, PlayerAmmoHUDClass);
	}
	OriginCameraRotation = MainCamera->GetRelativeRotation();
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
			PlayerWeapon[CurrentlySelectedWeapon]->FireDown(this);
		}
		else
		{
			Server_Fire(this, PlayerWeapon[CurrentlySelectedWeapon]);
		}
		
		Client_CallUpdateAmmo();
		OriginCameraRotation = ADSCamera->GetRelativeRotation();
		GetWorld()->GetTimerManager().ClearTimer(CameraResetTimerHandle);
		
	}
	
}

void AThirdPersonCharacter::HandleFireUp()
{
	if(!PlayerWeapon.IsEmpty())
	{
		if(HasAuthority())
		{
			PlayerWeapon[CurrentlySelectedWeapon]->FireUp();
		}
		else
		{
			Server_FireStop(PlayerWeapon[CurrentlySelectedWeapon]);
		}
		Client_CallUpdateAmmo();
		
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
	//UCharacterMovementComponent* CharMovementComp = GetCharacterMovement();
	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//BlendBetweenCamera(PlayerWeapon[CurrentlySelectedWeapon]);
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "OnADSComplete";
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();


	
	if(isADSed)
	{
		//BlendBetweenCamera(this);
		UKismetSystemLibrary::MoveComponentTo(MainCamera, FVector(50.0,114,130),FRotator(-15,0,0), false, true, 0.2f,true,EMoveComponentAction::Move, LatentInfo);
		ChangeMoveSpeed(500.0f);
		MainCamera->SetFieldOfView(90);
		//MainCamera->SetActive(true);
		//ADSCamera->SetActive(false);
		isADSed = false;
	}
	else
	{
		UKismetSystemLibrary::MoveComponentTo(MainCamera, FVector(400,0,60),FRotator(0,0,0), false, false, 0.2f,true,EMoveComponentAction::Move, LatentInfo);
		ChangeMoveSpeed(200.0f);
		MainCamera->SetFieldOfView(50);
		//BlendBetweenCamera(PlayerWeapon[CurrentlySelectedWeapon]);
		//ADSCamera->SetActive(true);
		//MainCamera->SetActive(false);
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

void AThirdPersonCharacter::Server_FireStop_Implementation(ABasePistol* Gun)
{
	Gun->FireUp();
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
}

void AThirdPersonCharacter::BlendBetweenCamera(AActor* GoToCam)
{
	auto temp = Cast<APlayerController>(GetController());
	temp->SetViewTargetWithBlend(GoToCam, 0.2f);
}

void AThirdPersonCharacter::OnADSComplete(bool RemoveScope)
{
	UE_LOG(LogTemp, Error, TEXT("ADS HAS COMPLETED !!! + ++ + ++ "));
	if(!ScopeWidget)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		ScopeWidget = CreateWidget<UUserWidget>(PC, ScopeWidgetClass);
	}
	
	if(isADSed)
	{
		ScopeWidget->AddToViewport();
	}
	else
	{
		ScopeWidget->RemoveFromParent();
		
	}
	
	
}

void AThirdPersonCharacter::Client_ResetRotateCamera_Implementation(float ResetTime)
{
	UE_LOG(LogTemp, Warning, TEXT("RESET HAS BEEN CALLED !!!"));
	GetWorld()->GetTimerManager().ClearTimer(CameraResetTimerHandle);
    
	FRotator InitialRotation = OriginCameraRotation;
	FQuat InitialQuat = InitialRotation.Quaternion();
	FQuat CurrentQuat = MainCamera->GetRelativeRotation().Quaternion();
    
	float StartTime = GetWorld()->GetTimeSeconds(); // Capture the starting time
    
	GetWorld()->GetTimerManager().SetTimer(CameraResetTimerHandle, [this, InitialQuat, CurrentQuat, ResetTime, StartTime]() mutable 
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float ElapsedTime = CurrentTime - StartTime; // Calculate elapsed time correctly
        
		float Alpha = FMath::Clamp(ElapsedTime / ResetTime, 0.0f, 1.0f);
		UE_LOG(LogTemp, Warning, TEXT("Elapsed Time: %f, Alpha: %f"), ElapsedTime, Alpha); // Debug Alpha value
        
		FQuat NewQuat = FQuat::Slerp(CurrentQuat, InitialQuat, Alpha);
		MainCamera->SetRelativeRotation(NewQuat.Rotator());
        
		if (ElapsedTime >= ResetTime)
		{
			GetWorld()->GetTimerManager().ClearTimer(CameraResetTimerHandle);
			MainCamera->SetRelativeRotation(InitialQuat.Rotator()); // Ensure it ends at the correct rotation
		}
	}, 0.01f, true);
}

void AThirdPersonCharacter::Client_RotateCamera_Implementation(float RotX, float RotY)
{
	UE_LOG(LogTemp, Error, TEXT("CLIENT ROTATION IS FIRING"));
	FRotator rotation = MainCamera->GetComponentRotation();
	FRotator temp = FRotator(rotation.Pitch += RotX, rotation.Yaw += RotY, rotation.Roll);
	MainCamera->SetWorldRotation(temp);
}

void AThirdPersonCharacter::Client_ScreenShake_Implementation(TSubclassOf<UCameraShakeBase> Shake)
{
	auto temp = Cast<APlayerController>(GetController());
	temp->ClientStartCameraShake(Shake, 10);
}

void AThirdPersonCharacter::Client_CallUpdateAmmo_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Update AMOOOOOO -----------------"));
	
	if(PlayerWeapon[CurrentlySelectedWeapon])
	{
		if(PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent)
		{
			if(PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->CurrentAmmo)
			{
				UpdateAmmoHUD(PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->CurrentAmmo,PlayerWeapon[CurrentlySelectedWeapon]->MagazineComponent->MaxAmmo);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Current Ammo -----------------"));
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No MagazineComp -----------------"));
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Weapon -----------------"));
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



