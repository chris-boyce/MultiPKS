// Fill out your copyright notice in the Description page of Project Settings.


#include "Blackhole.h"

#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

ABlackhole::ABlackhole()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABlackhole::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.0f);
	
}

void ABlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float Radius = 500.0f; 
	float Strength = 500.0f; 
	
	TArray<FHitResult> HitResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); 
	QueryParams.bReturnPhysicalMaterial = false;
	
	bool bResult = GetWorld()->SweepMultiByObjectType(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + FVector(1, 0, 0), 
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), 
		Sphere,
		QueryParams);

	if (bResult)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* Actor = Hit.GetActor();
			if (Actor && Actor->IsA(ABaseEnemy::StaticClass())) 
			{
				UCharacterMovementComponent* MoveComp = Actor->FindComponentByClass<UCharacterMovementComponent>();
				if (MoveComp)
				{
					FVector PullDirection = (GetActorLocation() - Actor->GetActorLocation()).GetSafeNormal();
					MoveComp->Velocity = PullDirection * Strength; 
				}
				
			}
		}
	}

}

