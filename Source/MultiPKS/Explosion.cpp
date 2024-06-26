// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion.h"

#include "Damageable.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"


AExplosion::AExplosion()
{
	bReplicates = true;
	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));
	ExplosionEffect->SetupAttachment(RootComponent);
	
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("ExplosionSound"));
	ExplosionSound->SetupAttachment(RootComponent);
	PrimaryActorTick.bCanEverTick = true;
	
}

void AExplosion::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AExplosion, DamageRadius);
	DOREPLIFETIME(AExplosion, ExplosionDamage);
	DOREPLIFETIME(AExplosion, ExplosionEffect);
	
}


void AExplosion::InitExplosion(float NewDamageRadius, float NewExplosionDamage)
{
	DamageRadius = NewDamageRadius;
	ExplosionDamage = NewExplosionDamage;

	if (ExplosionEffect)
	{
		// Assuming the default size of the particle system represents a radius of 100 units
		float ScaleFactor = DamageRadius / 100.0f;
		ExplosionEffect->SetWorldScale3D(FVector(ScaleFactor));
	}
	Explode();
}

float AExplosion::CalculateDamage(float Distance)
{
	if (Distance <= DamageRadius)
	{
		return ExplosionDamage * (1 - (Distance / DamageRadius));
	}
	return 0.0f; 
}

void AExplosion::BeginPlay()
{
	
	
}

void AExplosion::Explode()
{
	Super::BeginPlay();
	TArray<FHitResult> HitResults;
	TSet<AActor*> DamagedActors; 

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(DamageRadius);
	FCollisionQueryParams QueryParams(TEXT("ExplosionQuery"), true, this);

	GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation() + FVector(0.1f), // Minimal offset to ensure sweep works
		FQuat::Identity,
		ECC_WorldStatic,
		CollisionShape,
		QueryParams
	);

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<UDamageable>() && !DamagedActors.Contains(HitActor))
		{
			float Distance = FVector::Dist(Hit.ImpactPoint, GetActorLocation());
			float ScaledDamage = CalculateDamage(Distance);

			IDamageable* DamageableActor = Cast<IDamageable>(HitActor);
			if (DamageableActor)
			{
				DamageableActor->DetailedTakeDamage(ScaledDamage, Hit.ImpactPoint);
				DamagedActors.Add(HitActor); // Mark this actor as damaged
				UE_LOG(LogTemp, Warning, TEXT("Actor %s took %f damage"), *HitActor->GetName(), ScaledDamage);
			}
		}
	}
}

void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

