// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletSuctionComp.h"

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
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();  
	SpawnParams.Instigator = GetOwner()->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	auto temp = GetOwner()->GetWorld()->SpawnActor<ABlackhole>(BlackholeClass, ImpactPoint, FRotator::ZeroRotator, SpawnParams);
}
