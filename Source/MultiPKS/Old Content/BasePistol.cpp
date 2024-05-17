// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePistol.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MultiPKS/ThirdPersonCharacter.h"
#include "MultiPKS/WeaponDisplay.h"


ABasePistol::ABasePistol()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasePistol::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (MagazineClasses.Num() > 0 && EditMode == false)
	{
		int32 RandomIndex = FMath::RandRange(0, MagazineClasses.Num() - 1);
		TSubclassOf<AMagazine> SelectedMagazineClass = MagazineClasses[RandomIndex];
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		if(MagazineComponent)
		{
			MagazineComponent->Destroy();
		}
		MagazineComponent = GetWorld()->SpawnActor<AMagazine>(SelectedMagazineClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		MagazineComponent->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		
	}
}


void ABasePistol::BeginPlay()
{
	Super::BeginPlay();
	//SetMagDisplay();
}

void ABasePistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void ABasePistol::SetMagDisplay()
{
	
}

void ABasePistol::FireDown(AThirdPersonCharacter* FiringCharacter)
{
	if (FiringCharacter)
	{
		OriginRotation = FiringCharacter->ADSCamera->GetComponentRotation();
		GetWorld()->GetTimerManager().SetTimer(FiringTimerHandle, FTimerDelegate::CreateLambda([this, FiringCharacter]()
		{
			Fire(FiringCharacter);
			
		}), 0.2f, true);

		
	}
}

void ABasePistol::FireUp()
{
	GetWorld()->GetTimerManager().ClearTimer(FiringTimerHandle);
	//Cast<AThirdPersonCharacter>(GetOwner())->Client_ResetRotateCamera(1.0f);
	
	//Cast<AThirdPersonCharacter>(GetOwner())->Client_RotateCamera(OriginRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Firing stopped"));
	//PlayerCamera->SetWorldRotation(OriginRotation);
	
}

void ABasePistol::Fire(AThirdPersonCharacter* FiringCharacter)
{
	SetOwner(FiringCharacter);
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Proj Class"));
		return;
	}
	if(MagazineComponent->CurrentAmmo <= 0)
	{
		FiringCharacter->Client_CallUpdateAmmo();
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Passed Fire Function Tests"));

	MagazineComponent->ConsumeAmmo();
	if(HasAuthority())
	{
		FiringCharacter->Client_RotateCamera(1.0f, 0.0f);
		FiringCharacter->Client_CallUpdateAmmo();
		FiringCharacter->Client_ScreenShake(FiringCameraShake);
	}
	
	

	UE_LOG(LogTemp, Log, TEXT("Current Ammo: %d"), MagazineComponent->CurrentAmmo);
	
	APlayerController* PlayerController = Cast<APlayerController>(FiringCharacter->GetController());
	FRotator SpawnRotation = FiringCharacter->GetMainCameraComponent()->GetComponentRotation();
	FVector SpawnLocation = FiringCharacter->GetMainCameraComponent()->GetComponentLocation();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation , SpawnRotation , ActorSpawnParams);
	Multi_FireSound(SpawnLocation);
	
	
	
}

void ABasePistol::BindAmmoToHUD(AThirdPersonCharacter* Character)
{
	
}

void ABasePistol::UnBindAmmoToHUD(AThirdPersonCharacter* Character)
{
	
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








