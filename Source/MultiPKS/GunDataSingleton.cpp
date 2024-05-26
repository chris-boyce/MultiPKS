// Fill out your copyright notice in the Description page of Project Settings.


#include "GunDataSingleton.h"

#include <string>

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


FReturnWeaponData UGunDataSingleton::ReturnGunData() /* Random Gun Generation */
{
	FReturnWeaponData ReturnWeaponData;
	int32 MaxWeaponTypes = static_cast<int32>(EWeaponTypes::EWT_Pistol) + 1; 
	int32 RandomIndex = FMath::RandRange(0, MaxWeaponTypes - 1);
	EWeaponTypes WeaponType = static_cast<EWeaponTypes>(RandomIndex);
	
	UE_LOG(LogTemp, Log, TEXT("Selected Weapon Type: %d"), static_cast<int32>(WeaponType));
	
	for (auto WeaponData : MasterWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has No Entered Seed"));
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

FReturnWeaponData UGunDataSingleton::ReturnGunData(FString Seed) /* Seeded Gun Generation */
{
	FReturnWeaponData ReturnWeaponData;
	if(CheckSeedData(Seed)) //Checks FString that it is 6 digits //TODO : Add Alternative if isnt; 
	{
		for (auto DigitChar : Seed)
		{
			int Digit = DigitChar - '0';
			SeedArray.Add(Digit);
		}
		while (SeedArray[0] >= GetWeaponTypeCount())
		{
			SeedArray[0] = SeedArray[0] - GetWeaponTypeCount();
		}
		auto TempWeaponType = GetWeaponTypeByNumber(SeedArray[0]);
		
		for (auto WeaponData : MasterWeaponData)
		{
			if(WeaponData.WeaponType == TempWeaponType)
			{
				ReturnWeaponData.BaseMesh = WeaponData.BaseMesh;

				ReturnWeaponData.Magazine = GetElementFromArray(SeedArray[1], WeaponData.Magazines);
				
				ReturnWeaponData.Scope = GetElementFromArray(SeedArray[2], WeaponData.Scopes);

				ReturnWeaponData.Barrel = GetElementFromArray(SeedArray[3], WeaponData.Barrels);

				ReturnWeaponData.Muzzle = GetElementFromArray(SeedArray[4], WeaponData.Muzzles);

				ReturnWeaponData.Grips = GetElementFromArray(SeedArray[5], WeaponData.Grips);

				return ReturnWeaponData;
			}
		}
	}
	return ReturnWeaponData;
}

bool UGunDataSingleton::CheckSeedData(FString GunSeed)
{
	if (GunSeed.Len() != 6)
	{
		return false;
	}
	return true;
}

EWeaponTypes UGunDataSingleton::GetWeaponTypeByNumber(int32 Number)
{
	int32 MaxValue = static_cast<int32>(EWeaponTypes::EWeaponTypes_MAX);
	
	if (Number < 0 || Number >= MaxValue)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid number for weapon type: %d"), Number);
		return EWeaponTypes::EWT_Pistol;  
	}

	return static_cast<EWeaponTypes>(Number);
}

int UGunDataSingleton::GetWeaponTypeCount() const
{
	return static_cast<int>(EWeaponTypes::EWeaponTypes_MAX);
}


