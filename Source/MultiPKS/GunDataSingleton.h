// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GunDataSingleton.generated.h"


UENUM(BlueprintType)
enum class EWeaponTypes : uint8
{
	EWT_SMG UMETA(DisplayName = "SMG", Value = 1),
	EWT_Rifle UMETA(DisplayName = "Rifle", Value = 2),
	EWT_Sniper UMETA(DisplayName = "Sniper", Value = 3),
	EWT_Pistol UMETA(DisplayName = "Pistol", Value = 4),
	EWeaponTypes_MAX UMETA(Hidden) 
};

class AScope;
class AMuzzle;
class ABarrel;
class AGrip;
class AMagazine;
USTRUCT(BlueprintType)
struct FWeaponTypeStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun")
	EWeaponTypes WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	UStaticMesh* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Magazine")
	TArray<TSubclassOf<AMagazine>> Magazines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Scope")
	TArray<TSubclassOf<AScope>> Scopes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Barrel")
	TArray<TSubclassOf<ABarrel>> Barrels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Barrel")
	TArray<TSubclassOf<AMuzzle>> Muzzles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Barrel")
	TArray<TSubclassOf<AGrip>> Grips;
	
	FWeaponTypeStruct() : WeaponType(EWeaponTypes::EWT_Rifle), BaseMesh(nullptr)
	{
	}
};
USTRUCT(BlueprintType)
struct FReturnWeaponData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gun")
	UStaticMesh* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gun|ReturnData")
	TSubclassOf<AMagazine> Magazine;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gun|ReturnData")
	TSubclassOf<AScope> Scope;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gun|ReturnData")
	TSubclassOf<ABarrel> Barrel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gun|ReturnData")
	TSubclassOf<AMuzzle> Muzzle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Gun|ReturnData")
	TSubclassOf<AGrip> Grips;
};

class AScope;
class AMuzzle;
class ABarrel;
class AGrip;
class AMagazine;

UCLASS(Blueprintable)
class MULTIPKS_API UGunDataSingleton : public UObject
{
	GENERATED_BODY()
public:
	static UGunDataSingleton* Get();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|BaseMeshes")
	TArray<UStaticMesh*> BaseMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Data")
	int TempInt = 32;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gun|Data")
	TArray<FWeaponTypeStruct> MasterWeaponData;

	UFUNCTION(BlueprintCallable, Category="BaseMesh")
	void GetWeaponBaseMesh();
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	FReturnWeaponData ReturnGunData();

	FReturnWeaponData ReturnGunData(FString Seed);

	UFUNCTION()
	bool CheckSeedData(FString GunSeed);
	EWeaponTypes GetWeaponTypeByNumber(int32 Number);
	int32 GetWeaponTypeCount() const;

	UPROPERTY(VisibleAnywhere)
	TArray<int> SeedArray;

	UPROPERTY(VisibleAnywhere)
	TArray<int> AdjustedSeedArray;

	template<typename T>
	T GetElementFromArray(int32 Index, const TArray<T>& Array);

private:
	static UGunDataSingleton* SingletonInstance;
	
};

template <typename T>
T UGunDataSingleton::GetElementFromArray(int32 Index, const TArray<T>& Array)
{
	while (Index >= Array.Num())
	{
		Index = Index - Array.Num();
	}
	return Array[Index];
}
