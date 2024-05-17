// Fill out your copyright notice in the Description page of Project Settings.


#include "Magazine.h"

#include "Net/UnrealNetwork.h"


AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}


void AMagazine::BeginPlay()
{
	Super::BeginPlay();
	
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
	
	
}



