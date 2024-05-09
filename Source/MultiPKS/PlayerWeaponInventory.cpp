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

