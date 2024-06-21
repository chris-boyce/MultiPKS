// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletExplosionComp.h"

UBulletExplosionComp::UBulletExplosionComp()
{
}

void UBulletExplosionComp::BeginPlay()
{
	Super::BeginPlay();
}

void UBulletExplosionComp::HandleImpact(const FHitResult& HitResult)
{
	Explode(HitResult.Location);
	Super::HandleImpact(HitResult);
}

void UBulletExplosionComp::Explode(const FVector& HitLocation)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();  
	SpawnParams.Instigator = GetOwner()->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto temp = GetOwner()->GetWorld()->SpawnActor<AExplosion>(ExplosiveClass, HitLocation, FRotator::ZeroRotator, SpawnParams);
	temp->InitExplosion(100, 100);
}
