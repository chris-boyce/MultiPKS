// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Damageable.h"


ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");


	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	
	InitialLifeSpan = 3.0f;
}


void ABullet::BeginPlay()
{
	Super::BeginPlay();
}


void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABullet::InitializeVariables(float BulletDamage, float BulletVelocity)
{
	Damage = BulletDamage;
	ProjectileMovement->InitialSpeed = BulletVelocity;
	ProjectileMovement->MaxSpeed = BulletVelocity;
}

void ABullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	if(HasAuthority())
	{
		if(auto temp = Cast<IDamageable>(Other))
		{
			temp->TakeDamage(Damage);
			//DamageText(DamageAmount, Hit.Location);
			UE_LOG(LogTemp, Warning, TEXT("Has Been Hit : %s"), *Hit.BoneName.ToString());
		}
		
	}
}

