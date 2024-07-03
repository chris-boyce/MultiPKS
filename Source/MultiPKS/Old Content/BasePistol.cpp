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

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"


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
		if (!GunDataSingletonClass)
		{
			UE_LOG(LogTemp, Error, TEXT("Gun Data Singleton"));
			return;
		}
		GunDataSingleton = NewObject<UGunDataSingleton>(this, GunDataSingletonClass);
		if(GunIsRandom)
		{
			GunSeed = GenerateRandomSeed();
			GunData = ExtractGunDataFromSeed(GunSeed);
			ExtractValueDataFromSeed(GunSeed);
			
			for (int i = 1; i <= 5; ++i)
			{
				TransformedValueData.Add(ExponentialValueShift(ValueData[0], ValueData[i]));
			}
			
			WeaponData = GunDataSingleton->ReturnGunData(GunData);
		}
		else
		{
			GunData = ExtractGunDataFromSeed(GunSeed);
			ExtractValueDataFromSeed(GunSeed);
			
			for (int i = 1; i <= 5; ++i)
			{
				TransformedValueData.Add(ExponentialValueShift(ValueData[0], ValueData[i]));
			}
			
			WeaponData = GunDataSingleton->ReturnGunData(GunData);
		}

		if(ValueData[0] <= 25)
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding Mod to Weapon"));
			BulletModIndex = FMath::RandRange(0, BulletModClasses.Num() - 1);
			if(ValueData[0] <= 10) /* This is a "Do While" Loop --- New --- */
			{
				do
				{
					SecondBulletModIndex = FMath::RandRange(0, BulletModClasses.Num() - 1);
				}
				while (SecondBulletModIndex == BulletModIndex);
			}
			
		}
		
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
	MagazineComponent->AdjustScaleValue(TransformedValueData[0]);
		
	if(!WeaponData.Barrel)
	{
		UE_LOG(LogTemp, Error, TEXT("No Barrel Class"));
		Destroy();
		return;
	}
	BarrelComponent = GetWorld()->SpawnActor<ABarrel>(WeaponData.Barrel, GetActorLocation(), GetActorRotation(), SpawnParams);
	BarrelComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Barrel_Socket"));
	BarrelComponent->AdjustScaleValue(TransformedValueData[1]);
	if(!WeaponData.Scope)
	{
		UE_LOG(LogTemp, Error, TEXT("No Scope Class"));
		Destroy();
		return;
	}
	ScopeComponent = GetWorld()->SpawnActor<AScope>(WeaponData.Scope, GetActorLocation(), GetActorRotation(), SpawnParams);
	ScopeComponent->AttachToComponent(MainMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Scope_Socket"));
	ScopeComponent->AdjustScaleValue(TransformedValueData[2]);
	if(!WeaponData.Muzzle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Muzzle Class"));
		Destroy();
		return;
	}
	MuzzleComponent = GetWorld()->SpawnActor<AMuzzle>(WeaponData.Muzzle, GetActorLocation(), GetActorRotation(), SpawnParams);
	MuzzleComponent->AttachToComponent(BarrelComponent->StaticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Muzzle_Socket"));
	MuzzleComponent->AdjustScaleValue(TransformedValueData[3]);
	if(!WeaponData.Grips)
	{
		UE_LOG(LogTemp, Error, TEXT("No Grip Class"));
		Destroy();
		return;
	}
	GripComponent = GetWorld()->SpawnActor<AGrip>(WeaponData.Grips, GetActorLocation(), GetActorRotation(), SpawnParams);
	GripComponent->AttachToComponent(BarrelComponent->StaticMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Grip_Socket"));
	GripComponent->AdjustScaleValue(TransformedValueData[4]);

	WriteDataToCSV();

	
	
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
	DOREPLIFETIME(ABasePistol, ValueData);
	DOREPLIFETIME(ABasePistol, TransformedValueData);
	DOREPLIFETIME(ABasePistol, BulletModIndex);
	DOREPLIFETIME(ABasePistol, SecondBulletModIndex);
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

			WeaponDisplayWidget = CreateWidget<UWeaponDisplay>(PC, WeaponDisplay);

			if (WeaponDisplayWidget)
			{
				WeaponDisplayWidget->AddToViewport();
				WeaponDisplayWidget->TXT_GunValue->SetText(FText::AsNumber(ValueData[0]));
				WeaponDisplayWidget->BP_MagDisplay->SetAllText(MagazineComponent->AttachmentName, MagazineComponent->MaxAmmo, MagazineComponent->ReloadSpeed,FString("TBA"), MagazineComponent->ElementalPercentageChance, MagazineComponent->ElementalEffectTime, MagazineComponent->AttachmentScaleValue);
				WeaponDisplayWidget->BP_ScopeDisplay->SetAllText(ScopeComponent->AttachmentName, ScopeComponent->ADSSpeed, ScopeComponent->FOVChange, ScopeComponent->AttachmentValue);
				WeaponDisplayWidget->BP_BarrelDisplay->SetAllText(BarrelComponent->AttachmentName, BarrelComponent->FireRate, BarrelComponent->BulletDamage, BarrelComponent->FireMode, BarrelComponent->BurstSpeed, BarrelComponent->BurstCount, BarrelComponent->AttachementScale);
				WeaponDisplayWidget->BP_MuzzleDisplay->SetAllText(MuzzleComponent->AttachmentName, MuzzleComponent->FireSound.SoundLevel, MuzzleComponent->BulletVelocity, MuzzleComponent->AttachmentValue);
				WeaponDisplayWidget->BP_GripDisplay->SetAllText(GripComponent->AttachmentName, GripComponent->UnADSMoveSpeed, GripComponent->ADSedMoveSpeed, GripComponent->RecoilAmount, GripComponent->AttachmentValue);
				WeaponDisplayOnScreen = true;
			}
		}
	}
}

void ABasePistol::UnHighlightObject(AThirdPersonCharacter* InteractingCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Removing Widget"));
	DisableOutline();
	WeaponDisplayWidget->StartRemoveFromParent();
	//WeaponDisplayWidget = nullptr;
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

FString ABasePistol::GenerateRandomSeed()
{
	FString RandomString;
	RandomString.Reserve(18);
	
	for (int32 i = 0; i < 18; ++i)
	{
		int32 RandomDigit = FMath::RandRange(0, 9);  
		RandomString.AppendInt(RandomDigit);
	}
	
	return RandomString;
}

FString ABasePistol::ExtractGunDataFromSeed(const FString& OriginalString)
{
	FString Result;
	int DigitCount = 0;
	
	for (int i = 0; i < OriginalString.Len(); i++)
	{
		if (FChar::IsDigit(OriginalString[i]))
		{
			DigitCount++;
			
			if (DigitCount % 3 == 0)
			{
				Result.AppendChar(OriginalString[i]);
			}
		}
	}

	return Result;
}

void ABasePistol::ExtractValueDataFromSeed(const FString& OriginalString)
{

	if (OriginalString.Len() >= 18)
	{
		for (int i = 0; i < OriginalString.Len(); i += 3)
		{
			FString NumberString = OriginalString.Mid(i, 2);
			
			int32 Number = FCString::Atoi(*NumberString);
			ValueData.Add(Number);
		}
	}
}



int ABasePistol::ExponentialValueShift(int GunBaseValue, int AttachmentBaseValue)
{
	float Lambda = 0.03;
	float transformValueX = 99.0f * (1.0f - FMath::Exp(-Lambda * GunBaseValue));
	TransformedGunValue = transformValueX;
	float transformedValueY = (transformValueX / 99.0f) * 99.0f * (1.0f - FMath::Exp(-Lambda * AttachmentBaseValue));
	int RoundedTransformValue = round(transformedValueY);
	return RoundedTransformValue;
}

void ABasePistol::WriteDataToCSV()
{
	FString CSVFilePath = FPaths::ProjectDir() + TEXT("PistolStats.csv");
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*CSVFilePath);
	FString CSVData;

	if (!bFileExists)
	{
		CSVData += TEXT("Seed,GV,TV1,TV2,TV3,TV4,TV5,BM1,BM2\n");
	}
	
	CSVData += FString::Printf(TEXT("%s,"), *GunSeed);
	CSVData += FString::Printf(TEXT("%d,"), TransformedGunValue);
	
	for (int32 Value : TransformedValueData)
	{
		CSVData += FString::Printf(TEXT("%d,"), Value);
	}
	
	CSVData += FString::Printf(TEXT("%d,"), BulletModIndex);
	CSVData += FString::Printf(TEXT("%d"), SecondBulletModIndex);
	CSVData += TEXT("\n");
	
	FFileHelper::SaveStringToFile(CSVData, *CSVFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
	
}

void ABasePistol::HideWeaponModel(bool shouldHide)
{
	BarrelComponent->StaticMeshComponent->SetVisibility(shouldHide);
	//MuzzleComponent->StaticMeshComponent->SetVisibility(shouldHide);
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
	BarrelComponent->BulletModIndex = BulletModIndex;
	BarrelComponent->SecondBulletModIndex = SecondBulletModIndex;
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








