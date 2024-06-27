// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBounceComp.h"

#include "Bullet.h"

UBulletBounceComp::UBulletBounceComp()
{
}

void UBulletBounceComp::BeginPlay()
{
	Super::BeginPlay();
}

void UBulletBounceComp::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UBulletBounceComp::HandleImpact(const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("has hit"));
	if(HasBounced == true)
	{
		auto temp = Cast<ABullet>(GetOwner());
		temp->DoDamage(HitResult.GetActor(), HitResult);
		UE_LOG(LogTemp, Warning, TEXT("Im Gone"));
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("has Yooo"));
		auto temp = Cast<ABullet>(GetOwner());
		temp->DoDamage(HitResult.GetActor(), HitResult);
		HasBounced = true;
		return false;
	}
	
	
	
}
