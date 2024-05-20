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
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		if (MagazineClasses.Num() > 0 && EditMode == false)
		{
			TSubclassOf<AMagazine> SelectedMagazineClass = MagazineClasses[FMath::RandRange(0, MagazineClasses.Num() - 1)];
			MagazineComponent = GetWorld()->SpawnActor<AMagazine>(SelectedMagazineClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			MagazineComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Mag_Socket"));
		}
		
		if(BarrelClasses.Num() > 0 && EditMode == false) /*Offset is needed due to model being broken | TODO : Fix This When Real Model Used*/
		{
			TSubclassOf<ABarrel> SelectedBarrelClass = BarrelClasses[FMath::RandRange(0, BarrelClasses.Num() - 1)];
			BarrelComponent = GetWorld()->SpawnActor<ABarrel>(SelectedBarrelClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			BarrelComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Barrel_Socket"));
		}
		if(ScopeClasses.Num() > 0 && EditMode == false)
		{
			TSubclassOf<AScope> SelectedScopeClass = ScopeClasses[FMath::RandRange(0, ScopeClasses.Num() - 1)];
			ScopeComponent = GetWorld()->SpawnActor<AScope>(SelectedScopeClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			ScopeComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Scope_Socket"));
		}
		if(MuzzleClasses.Num() > 0 && EditMode == false)
		{
			TSubclassOf<AMuzzle> SelectedMuzzleClass = MuzzleClasses[FMath::RandRange(0, MuzzleClasses.Num() - 1)];
			MuzzleComponent = GetWorld()->SpawnActor<AMuzzle>(SelectedMuzzleClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			MuzzleComponent->AttachToComponent(BarrelComponent->StaticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Muzzle_Socket"));
		}
		if(GripClasses.Num() > 0 && EditMode == false)
		{
			TSubclassOf<AGrip> SelectedGripClass = GripClasses[FMath::RandRange(0, GripClasses.Num() - 1)];
			GripComponent = GetWorld()->SpawnActor<AGrip>(SelectedGripClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			GripComponent->AttachToComponent(BarrelComponent->StaticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Grip_Socket"));
		}
		
	}
	if (GunDataSingletonClass)
	{
		GunDataSingleton = NewObject<UGunDataSingleton>(this, GunDataSingletonClass);
		UE_LOG(LogTemp, Warning, TEXT("TestInt value: %d"), GunDataSingleton->TempInt);
		
	}
	
}

void ABasePistol::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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








