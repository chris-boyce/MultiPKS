// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeaponInventory.h"


UPlayerWeaponInventory::UPlayerWeaponInventory()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}



void UPlayerWeaponInventory::BeginPlay()
{
	Super::BeginPlay();
	
}



void UPlayerWeaponInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UPlayerWeaponInventory::AddWeaponToInventory(ABasePistol* Gun)
{
	GunInventory.Add(Gun);
	OnUpdateInventory.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Has Added to Invetory and fired broadcast"));
}

