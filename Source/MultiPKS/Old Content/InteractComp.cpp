// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComp.h"

#include "BasePistol.h"
#include "Components/SphereComponent.h"
#include "MultiPKS/Pickupable.h"
#include "MultiPKS/ThirdPersonCharacter.h"

UInteractComp::UInteractComp()
{
	
}

void UInteractComp::InteractWithObject()
{
	auto Controller = Cast<AThirdPersonCharacter>(GetOwner())->GetLocalViewingPlayerController();
	
	if (Controller)
	{
		FVector StartLocation;
		FRotator ViewRotation;
		Controller->GetPlayerViewPoint(StartLocation, ViewRotation);

		FVector EndLocation = StartLocation + (ViewRotation.Vector() * 500); // Trace 1000 units forward

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

		if (bHit)
		{
			//DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);
			IPickupable* Pickupable = Cast<IPickupable>(HitResult.GetActor());
			if (Pickupable)
			{
				auto gun  = Pickupable->PickupObject(Cast<AThirdPersonCharacter>(GetOwner()));
				if(gun)
				{
					if(GetOwner()->HasAuthority())
					{
						Multi_AttachGun(gun);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("RUNNNING PICK THROUGH SERVER"));
						Server_AttachGun(gun);
					}
					
				}
			}
		}
	}
}

void UInteractComp::Server_AttachGun_Implementation(ABasePistol* Gun)
{
	Multi_AttachGun(Gun);
}

bool UInteractComp::Server_AttachGun_Validate(ABasePistol* Gun)
{
	return true;
}


void UInteractComp::Multi_AttachGun_Implementation(ABasePistol* Gun)
{
	UE_LOG(LogTemp, Warning, TEXT("Has Gun At Multi Check: %s"), *Gun->GetName());
	
	Gun->AttachToComponent(Cast<AThirdPersonCharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GunSocket"));
	Gun->SphereComponentZ->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	auto Owner = Cast<AThirdPersonCharacter>(GetOwner());
	if(Owner->PlayerWeapon.Num() <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding Weapon To PLayer List"));
		Owner->PlayerWeapon.Add(Gun);
		LogWeaponPickup(Gun, -1);
	}
	else
	{
		DropWeapon.Broadcast(Cast<AThirdPersonCharacter>(GetOwner()));
		UE_LOG(LogTemp, Warning, TEXT("Fired BroadCast"));
		Owner->PlayerWeapon[Owner->CurrentlySelectedWeapon] = Gun;
		LogWeaponPickup(Gun, Owner->CurrentlySelectedWeapon);
	}
	
	
}

bool UInteractComp::Multi_AttachGun_Validate(ABasePistol* Gun)
{
	return true;
}

void UInteractComp::LogWeaponPickup(ABasePistol* Gun, int Slot)
{
	FString CSVFilePath = FPaths::ProjectDir() + TEXT("PickupGun.csv");
	bool bFileExists = FPlatformFileManager::Get().GetPlatformFile().FileExists(*CSVFilePath);
	FString CSVData;

	if (!bFileExists)
	{
		CSVData += TEXT("Time,Seed,WeaponType,Slot,BulletMod1,BulletMod2,MapName\n");
	}
	
	FDateTime CurrentTime = FDateTime::Now();
	FString TimeString = CurrentTime.ToString(TEXT("%d %H:%M:%S"));
	
	
	CSVData += FString::Printf(TEXT("%s,"), *TimeString);
	CSVData += FString::Printf(TEXT("%s,"), *Gun->GunSeed);
	CSVData += FString::Printf(TEXT("%d,"), Slot);
	CSVData += FString::Printf(TEXT("%s,"), *Gun->WeaponData.BaseMesh->GetName());
	CSVData += FString::Printf(TEXT("%d,"), Gun->BulletModIndex);
	CSVData += FString::Printf(TEXT("%d,"), Gun->SecondBulletModIndex);
	CSVData += FString::Printf(TEXT("%s,"), *GetWorld()->GetMapName());
	CSVData += TEXT("\n");
	
	FFileHelper::SaveStringToFile(CSVData, *CSVFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

}

void UInteractComp::PerformLineTrace()
{
	auto Controller = Cast<AThirdPersonCharacter>(GetOwner())->GetLocalViewingPlayerController();
	
	if (Controller)
	{
		FVector StartLocation;
		FRotator ViewRotation;
		Controller->GetPlayerViewPoint(StartLocation, ViewRotation);

		FVector EndLocation = StartLocation + (ViewRotation.Vector() * 500); // Trace 1000 units forward

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

		if (bHit)
		{
			//DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);
			SavedPickupable = Cast<IPickupable>(HitResult.GetActor());
			if (SavedPickupable)
			{
				SavedPickupable->HighlightObject(Cast<AThirdPersonCharacter>(GetOwner()));
			}
			else
			{
				if(SavedPickupable)
				{
					SavedPickupable->UnHighlightObject(Cast<AThirdPersonCharacter>(GetOwner()));
					SavedPickupable  = nullptr;
				}
				
			}
			
		}
		else
		{
			if(SavedPickupable)
			{
				SavedPickupable->UnHighlightObject(Cast<AThirdPersonCharacter>(GetOwner()));
				SavedPickupable  = nullptr;
			}
			
		}
	}
}


void UInteractComp::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInteractComp::PerformLineTrace, 0.05f, true);
	
}





