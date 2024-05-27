// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAdjustmentPlate.h"

#include "Damageable.h"


AHealthAdjustmentPlate::AHealthAdjustmentPlate()
{
 
	PrimaryActorTick.bCanEverTick = true;

}

void AHealthAdjustmentPlate::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthAdjustmentPlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthAdjustmentPlate::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if(HasAuthority())
	{
		if(auto temp = Cast<IDamageable>(Other))
		{
			temp->TakeDamage(Amount);
			UE_LOG(LogTemp, Warning, TEXT("Has Been Hit : %s"), *Hit.BoneName.ToString());
		}
		
	}
}



