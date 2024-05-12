// Fill out your copyright notice in the Description page of Project Settings.


#include "Magazine.h"


AMagazine::AMagazine()
{
	PrimaryActorTick.bCanEverTick = true;
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
	CurrentAmmo = CurrentAmmo - 1;
}

void AMagazine::ReloadMag()
{
	isReloading = true;
	GetWorldTimerManager().SetTimer(ReloadTimeHandle, this, &AMagazine::HandleReloadComplete, ReloadSpeed, false);
}


