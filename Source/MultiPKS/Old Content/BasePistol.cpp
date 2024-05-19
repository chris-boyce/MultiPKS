// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePistol.h"


#include "Kismet/GameplayStatics.h"
#include "MultiPKS/Barrel.h"
#include "MultiPKS/BarrelDisplay.h"
#include "MultiPKS/MagDisplay.h"
#include "MultiPKS/Scope.h"
#include "MultiPKS/ScopeDisplay.h"
#include "MultiPKS/ThirdPersonCharacter.h"
#include "MultiPKS/WeaponDisplay.h"
#include "Net/UnrealNetwork.h"


ABasePistol::ABasePistol()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	
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
		if (MagazineClasses.Num() > 0 && EditMode == false)
		{
			TSubclassOf<AMagazine> SelectedMagazineClass = MagazineClasses[FMath::RandRange(0, MagazineClasses.Num() - 1)];
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
		
			MagazineComponent = GetWorld()->SpawnActor<AMagazine>(SelectedMagazineClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			MagazineComponent->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		
		if(BarrelClasses.Num() > 0 && EditMode == false)
		{
			TSubclassOf<ABarrel> SelectedBarrelClass = BarrelClasses[FMath::RandRange(0, BarrelClasses.Num() - 1)];
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			FVector RelativeOffset(0, 50, 0); // Offset in local space
			FVector WorldOffset = GetActorTransform().TransformVector(RelativeOffset);

			BarrelComponent = GetWorld()->SpawnActor<ABarrel>(SelectedBarrelClass, GetActorLocation() + WorldOffset, GetActorRotation(), SpawnParams);
			BarrelComponent->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		if(ScopeClasses.Num() > 0 && EditMode == false)
		{
			TSubclassOf<AScope> SelectedScopeClass = ScopeClasses[FMath::RandRange(0, ScopeClasses.Num() - 1)];
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			ScopeComponent = GetWorld()->SpawnActor<AScope>(SelectedScopeClass, GetActorLocation(), GetActorRotation(), SpawnParams);
			ScopeComponent->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	
	
	
}

void ABasePistol::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABasePistol, MagazineComponent);
	DOREPLIFETIME(ABasePistol, BarrelComponent);
	DOREPLIFETIME(ABasePistol, ScopeComponent);
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
	BarrelComponent->Fire(FiringCharacter, ProjectileClass, MagazineComponent);


	
	//Multi_FireSound(SpawnLocation);
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








