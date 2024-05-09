// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMagazine.h"

UBaseMagazine::UBaseMagazine()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}


void UBaseMagazine::BeginPlay()
{
	Super::BeginPlay();
	
	
}



void UBaseMagazine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

