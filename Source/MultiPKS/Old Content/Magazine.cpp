// Fill out your copyright notice in the Description page of Project Settings.


#include "Magazine.h"

#include "Net/UnrealNetwork.h"


AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	CurrentAmmoRange = FIntPoint(5, 50);
	MaxAmmoRange = FIntPoint(5, 50);
	ReloadSpeedRange = FVector2D(0.5f, 3.0f);
	ElementalPercentageChanceRange = FVector2D(1.0f, 25.0f);
	ElementalEffectTimeRange =  FVector2D(1.0f, 5.0f);
}


void AMagazine::BeginPlay()
{
	Super::BeginPlay();
	
}



void AMagazine::AdjustScaleValue(int Scale)
{
	AttachmentScaleValue = Scale;
	float ScaleAlpha = static_cast<float>(Scale) / 100.0f;
	CurrentAmmo = FMath::Lerp(CurrentAmmoRange.X, CurrentAmmoRange.Y, ScaleAlpha);
	MaxAmmo = FMath::Lerp(MaxAmmoRange.X, MaxAmmoRange.Y, ScaleAlpha);
	ReloadSpeed = FMath::Lerp(ReloadSpeedRange.X, ReloadSpeedRange.Y, ScaleAlpha);
	ElementalPercentageChance = FMath::Lerp(ElementalPercentageChanceRange.X, ElementalPercentageChanceRange.Y, ScaleAlpha);
	ElementalEffectTime =  FMath::Lerp(ElementalEffectTimeRange.X, ElementalEffectTimeRange.Y, ScaleAlpha);
}

bool AMagazine::CanFire()
{
	if(CurrentAmmo <= 0 || isReloading == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void AMagazine::HandleReloadComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("Reload complete!"));
	isReloading = false;
	CurrentAmmo = MaxAmmo;
	OnReloadComplete.Broadcast(CurrentAmmo, MaxAmmo);
}

void AMagazine::ConsumeAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("Cosume Ammo"));
	UE_LOG(LogTemp, Warning, TEXT("Updated Ammo On  Called Locally"));
	CurrentAmmo = CurrentAmmo - 1;
}

void AMagazine::ReloadMag()
{
	isReloading = true;
	GetWorldTimerManager().SetTimer(ReloadTimeHandle, this, &AMagazine::HandleReloadComplete, ReloadSpeed, false);
}

void AMagazine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagazine, CurrentAmmo);
	DOREPLIFETIME(AMagazine, MaxAmmo);
	DOREPLIFETIME(AMagazine, ReloadSpeed);
	DOREPLIFETIME(AMagazine, AttachmentScaleValue);
	DOREPLIFETIME(AMagazine, BulletElementalType);
	DOREPLIFETIME(AMagazine, ElementalEffectTime);
	DOREPLIFETIME(AMagazine, ElementalPercentageChance);
	
	
}



