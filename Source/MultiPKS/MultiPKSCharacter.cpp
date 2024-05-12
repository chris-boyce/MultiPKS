// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiPKSCharacter.h"

#include "BasePistol.h"
#include "MultiPKSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PickupComp.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMultiPKSCharacter

AMultiPKSCharacter::AMultiPKSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	WeaponInventory = CreateDefaultSubobject<UPlayerWeaponInventory>(TEXT("Weapon Inventory"));
	WeaponInventory->OnUpdateInventory.AddDynamic(this, &AMultiPKSCharacter::HandleInventoryUpdate);
	UE_LOG(LogTemp, Warning, TEXT("Has Bound The Inventory"));

}

void AMultiPKSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMultiPKSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiPKSCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiPKSCharacter::Look);
		
		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMultiPKSCharacter::Interact);

		//Fire
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMultiPKSCharacter::Fire);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMultiPKSCharacter::HandleCrouch);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AMultiPKSCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMultiPKSCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMultiPKSCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact Fired"));
	FVector StartPoint = GetActorLocation(); 
	FVector ForwardVector = GetActorForwardVector(); 
	FVector EndPoint = StartPoint + ForwardVector * 1000.0f; 
	FHitResult HitResult;
	
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this); 
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECC_Visibility, 
		QueryParams
	);
	
	DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 2.0f);

	if (bHit && HitResult.GetActor())
	{
		UPickupComp* PickupComp = Cast<UPickupComp>(HitResult.GetActor()->GetComponentByClass(UPickupComp::StaticClass()));
        
		if (PickupComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Hit"));
			if (HasAuthority()) 
			{
				PickupComp->AttachGun(this);
			}
			else
			{
				Server_Interact(PickupComp);
			}
		}
	}
}

void AMultiPKSCharacter::Fire()
{
	if(!WeaponInventory->GunInventory.IsEmpty())
	{
		if(HasAuthority())
		{
			WeaponInventory->GunInventory[0]->Fire();
		}
		else
		{
			Server_Fire(WeaponInventory->GunInventory[0]);
		}
	}
}

void AMultiPKSCharacter::HandleCrouch()
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

void AMultiPKSCharacter::Server_Fire_Implementation(ABasePistol* BasePistol)
{
	if(BasePistol)
	{
		BasePistol->Fire();
	}
}

void AMultiPKSCharacter::HandleInventoryUpdate()
{
	UE_LOG(LogTemp, Warning, TEXT("BroadCast Recieved Firing Blueprint"));
	isArmed = true;
	IsArmedUpdate();
}

bool AMultiPKSCharacter::Server_Fire_Validate(ABasePistol* BasePistol)
{
	return true;
}


void AMultiPKSCharacter::Server_Interact_Implementation(UPickupComp* PickupComp)
{
	if(PickupComp)
	{
		PickupComp->AttachGun(this);
	}
}

bool AMultiPKSCharacter::Server_Interact_Validate(UPickupComp* PickupComp)
{
	return true;
}




