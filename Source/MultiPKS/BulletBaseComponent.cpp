// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBaseComponent.h"


UBulletBaseComponent::UBulletBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UBulletBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void UBulletBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

bool UBulletBaseComponent::HandleImpact(const FHitResult& HitResult)
{
	return true;
}

void UBulletBaseComponent::HandleMovement()
{
	
}

