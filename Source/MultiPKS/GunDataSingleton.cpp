// Fill out your copyright notice in the Description page of Project Settings.


#include "GunDataSingleton.h"

UGunDataSingleton* UGunDataSingleton::SingletonInstance = nullptr;

UGunDataSingleton* UGunDataSingleton::Get()
{
	if (!SingletonInstance)
	{
		SingletonInstance = NewObject<UGunDataSingleton>();
		SingletonInstance->AddToRoot();  // Prevent garbage collection
	}
	return SingletonInstance;
}

void UGunDataSingleton::GetWeaponBaseMesh()
{
	for (FWeaponTypeStruct& WeaponStruct : MasterWeaponData)
	{
		if (BaseMeshes.Num() == 0) return;

		switch (WeaponStruct.WeaponType)
		{
		case EWeaponTypes::EWT_SMG:
			WeaponStruct.BaseMesh = BaseMeshes.IsValidIndex(0) ? BaseMeshes[0] : nullptr;
			break;
		case EWeaponTypes::EWT_Rifle:
			WeaponStruct.BaseMesh = BaseMeshes.IsValidIndex(1) ? BaseMeshes[1] : nullptr;
			break;
		case EWeaponTypes::EWT_Sniper:
			WeaponStruct.BaseMesh = BaseMeshes.IsValidIndex(2) ? BaseMeshes[2] : nullptr;
			break;
		case EWeaponTypes::EWT_Pistol:
			WeaponStruct.BaseMesh = BaseMeshes.IsValidIndex(3) ? BaseMeshes[3] : nullptr;
			break;
		default:
			WeaponStruct.BaseMesh = nullptr;
			break;
		}
	}
}

void UGunDataSingleton::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	UE_LOG(LogTemp, Warning, TEXT("Updated Property Called"));
	GetWeaponBaseMesh();
}

FReturnWeaponData UGunDataSingleton::ReturnGunData(EWeaponTypes WeaponType)
{
	FReturnWeaponData ReturnWeaponData;
	for (auto WeaponData : MasterWeaponData)
	{
		if(WeaponData.WeaponType == WeaponType)
		{
			ReturnWeaponData.BaseMesh = WeaponData.BaseMesh;
			
			ReturnWeaponData.Magazine = WeaponData.Magazines[FMath::RandRange(0, WeaponData.Magazines.Num() - 1)];

			ReturnWeaponData.Scope = WeaponData.Scopes[FMath::RandRange(0, WeaponData.Scopes.Num() - 1)];

			ReturnWeaponData.Barrel = WeaponData.Barrels[FMath::RandRange(0, WeaponData.Barrels.Num() - 1)];

			ReturnWeaponData.Muzzle = WeaponData.Muzzles[FMath::RandRange(0, WeaponData.Muzzles.Num() - 1)];

			ReturnWeaponData.Grips = WeaponData.Grips[FMath::RandRange(0, WeaponData.Grips.Num() - 1)];

			return ReturnWeaponData;
			
		}
	}
	return ReturnWeaponData;
}
