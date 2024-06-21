// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletExplosive.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABulletExplosive::ABulletExplosive()
{
	PrimaryActorTick.bCanEverTick = true;

	
}

void ABulletExplosive::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletExplosive::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
}

void ABulletExplosive::Explode(const FVector& HitLocation)
{
	
}
