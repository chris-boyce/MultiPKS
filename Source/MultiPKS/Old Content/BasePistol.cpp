// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePistol.h"


#include "Kismet/GameplayStatics.h"
#include "MultiPKS/Barrel.h"
#include "MultiPKS/BarrelDisplay.h"
#include "MultiPKS/Grip.h"
#include "MultiPKS/GripDisplay.h"
#include "MultiPKS/MagDisplay.h"
#include "MultiPKS/Muzzle.h"
#include "MultiPKS/MuzzleDisplay.h"
#include "MultiPKS/Scope.h"
#include "MultiPKS/ScopeDisplay.h"
#include "MultiPKS/ThirdPersonCharacter.h"
#include "MultiPKS/WeaponDisplay.h"
#include "Net/UnrealNetwork.h"


ABasePistol::ABasePistol()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = MainMesh;
}

void ABasePistol::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


void ABasePistol::BeginPlay()
{
	Super::BeginPlay();
	if(HasAuthority())
	{
		int32 MaxWeaponTypes = static_cast<int32>(EWeaponTypes::EWT_Pistol) + 1; 
		int32 RandomIndex = FMath::RandRange(0, MaxWeaponTypes - 1);
		WeaponType = static_cast<EWeaponTypes>(RandomIndex);
		UE_LOG(LogTemp, Log, TEXT("Selected Weapon Type: %d"), static_cast<int32>(WeaponType));
		if (!GunDataSingletonClass)
		{
			UE_LOG(LogTemp, Error, TEXT("Gun Data Singleton"));
			return;
		}
	
		GunDataSingleton = NewObject<UGunDataSingleton>(this, GunDataSingletonClass);
		UE_LOG(LogTemp, Warning, TEXT("TestInt value: %d"), GunDataSingleton->TempInt);
		WeaponData = GunDataSingleton->ReturnGunData(WeaponType);
	}
	
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	MainMesh->SetStaticMesh(WeaponData.BaseMesh);
	
	if(!WeaponData.Magazine)
	{
		UE_LOG(LogTemp, Error, TEXT("No MagazineClass"));
		Destroy();
		return;
	}
	MagazineComponent = GetWorld()->SpawnActor<AMagazine>(WeaponData.Magazine, GetActorLocation(), GetActorRotation(), SpawnParams);
	MagazineComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Mag_Socket"));
		
	if(!WeaponData.Barrel)
	{
		UE_LOG(LogTemp, Error, TEXT("No Barrel Class"));
		Destroy();
		return;
	}
	BarrelComponent = GetWorld()->SpawnActor<ABarrel>(WeaponData.Barrel, GetActorLocation(), GetActorRotation(), SpawnParams);
	BarrelComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Barrel_Socket"));

	if(!WeaponData.Scope)
	{
		UE_LOG(LogTemp, Error, TEXT("No Scope Class"));
		Destroy();
		return;
	}
	ScopeComponent = GetWorld()->SpawnActor<AScope>(WeaponData.Scope, GetActorLocation(), GetActorRotation(), SpawnParams);
	ScopeComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Scope_Socket"));
		
	if(!WeaponData.Muzzle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Muzzle Class"));
		Destroy();
		return;
	}
	MuzzleComponent = GetWorld()->SpawnActor<AMuzzle>(WeaponData.Muzzle, GetActorLocation(), GetActorRotation(), SpawnParams);
	MuzzleComponent->AttachToComponent(BarrelComponent->StaticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Muzzle_Socket"));
		
	if(!WeaponData.Grips)
	{
		UE_LOG(LogTemp, Error, TEXT("No Grip Class"));
		Destroy();
		return;
	}
	GripComponent = GetWorld()->SpawnActor<AGrip>(WeaponData.Grips, GetActorLocation(), GetActorRotation(), SpawnParams);
	GripComponent->AttachToComponent(BarrelComponent->StaticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Grip_Socket"));

	
}

void ABasePistol::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABasePistol, MainMesh);
	DOREPLIFETIME(ABasePistol, WeaponType);
	DOREPLIFETIME(ABasePistol, WeaponData);
	DOREPLIFETIME(ABasePistol, MagazineComponent);
	DOREPLIFETIME(ABasePistol, BarrelComponent);
	DOREPLIFETIME(ABasePistol, ScopeComponent);
	DOREPLIFETIME(ABasePistol, MuzzleComponent);
	DOREPLIFETIME(ABasePistol, GripComponent);
}


void ABasePistol::HighlightObject(AThirdPersonCharacter* InteractingCharacter)
{
	EnableOutline();
	if(WeaponDisplayOnScreen)
	{
		return;
	}
	
	if (InteractingCharacter)
	{
		APlayerController* PC = Cast<APlayerController>(InteractingCharacter->GetController());
		if (PC && WeaponDisplay)  
		{

			WeaponDisplayWidget = CreateWidget<UUserWidget>(PC, WeaponDisplay);

			if (WeaponDisplayWidget)
			{
				WeaponDisplayWidget->AddToViewport();
				Cast<UWeaponDisplay>(WeaponDisplayWidget)->BP_MagDisplay->SetAllText(MagazineComponent->GetName(), FString("TBA"), MagazineComponent->MaxAmmo, MagazineComponent->ReloadSpeed, FString("TBA"), MagazineComponent->ElementalPercentageChance, MagazineComponent->ElementalEffectTime );
				Cast<UWeaponDisplay>(WeaponDisplayWidget)->BP_ScopeDisplay->SetAllText(ScopeComponent->GetName(), ScopeComponent->ADSSpeed, ScopeComponent->FOVChange);
				Cast<UWeaponDisplay>(WeaponDisplayWidget)->BP_BarrelDisplay->SetAllText(BarrelComponent->GetName(), BarrelComponent->FireRate, BarrelComponent->BulletDamage, BarrelComponent->FireMode, BarrelComponent->BurstSpeed, BarrelComponent->BurstCount);
				Cast<UWeaponDisplay>(WeaponDisplayWidget)->BP_MuzzleDisplay->SetAllText(MuzzleComponent->GetName(), MuzzleComponent->FireSound.SoundLevel, MuzzleComponent->BulletVelocity);
				Cast<UWeaponDisplay>(WeaponDisplayWidget)->BP_GripDisplay->SetAllText(GripComponent->GetName(), GripComponent->UnADSMoveSpeed, GripComponent->ADSedMoveSpeed, GripComponent->RecoilAmount);
				WeaponDisplayOnScreen = true;
			}
		}
	}
}

void ABasePistol::UnHighlightObject(AThirdPersonCharacter* InteractingCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Removing Widget"));
	DisableOutline();
	WeaponDisplayWidget->RemoveFromParent();
	WeaponDisplayWidget = nullptr;
	WeaponDisplayOnScreen = false;
}

ABasePistol* ABasePistol::PickupObject(AThirdPersonCharacter* InteractingCharacter)
{
	
	if(!InteractingCharacter->GetPlayerAmmoHUD()->IsInViewport())
	{
		InteractingCharacter->GetPlayerAmmoHUD()->AddToViewport();
	}
	InteractingCharacter->UpdateAmmoHUD(MagazineComponent->CurrentAmmo, MagazineComponent->MaxAmmo);
	
	
	UE_LOG(LogTemp, Warning, TEXT("Gun being returned: %s"), *this->GetName());
	return this;
}



void ABasePistol::SetMagDisplay(AThirdPersonCharacter* InteractingCharacter)
{
	InteractingCharacter->Client_CallUpdateAmmo(MagazineComponent->CurrentAmmo, MagazineComponent->MaxAmmo);
}

void ABasePistol::FireDown(AThirdPersonCharacter* FiringCharacter)
{
	if(FiringCharacter)
	{
		if(BarrelComponent->FireMode == EFireMode::Automatic)
		{
			Fire(FiringCharacter);
			GetWorld()->GetTimerManager().SetTimer(FiringTimerHandle, FTimerDelegate::CreateLambda([this, FiringCharacter]()
				{
					Fire(FiringCharacter);
			
				}), BarrelComponent->FireRate, true);
		}
		else
		{
			Fire(FiringCharacter);
		}
	}
	
	
}

void ABasePistol::FireUp(AThirdPersonCharacter* FiringCharacter)
{
	GetWorld()->GetTimerManager().ClearTimer(FiringTimerHandle);
	FiringCharacter->ResetRotateCamera(1.0f);
}

void ABasePistol::Fire(AThirdPersonCharacter* FiringCharacter)
{
	if (!ensure(ProjectileClass != nullptr))
	{
		return;
	}
	
	FiringCharacter->Client_ScreenShake(FiringCameraShake);
	BarrelComponent->Fire(FiringCharacter, ProjectileClass, MagazineComponent, GripComponent, MuzzleComponent);
	


}


void ABasePistol::Multi_FireSound_Implementation(FVector Location)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered Sound Function"));
	if (FireSound != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing Sound"));
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Location);
	}
}

bool ABasePistol::Multi_FireSound_Validate(FVector Location)
{
	return true;
}








