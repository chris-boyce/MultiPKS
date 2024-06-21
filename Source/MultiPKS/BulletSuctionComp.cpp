// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletSuctionComp.h"

#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UBulletSuctionComp::UBulletSuctionComp()
{
}

void UBulletSuctionComp::BeginPlay()
{
	Super::BeginPlay();
}

void UBulletSuctionComp::HandleImpact(const FHitResult& HitResult)
{
	Super::HandleImpact(HitResult);
	ApplySuction(HitResult.Location);
}

void UBulletSuctionComp::ApplySuction(const FVector& ImpactPoint)
{
	
}
