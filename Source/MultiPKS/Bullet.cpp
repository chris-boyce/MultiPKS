// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "BulletMagneticComp.h"
#include "Damageable.h"
#include "Net/UnrealNetwork.h"


ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
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
	if(isGotBulletMod)
	{
		if( BulletMod->IsA(UBulletMagneticComp::StaticClass()))
		{
			BulletMod->HandleMovement();
		}
		if(SecondBulletMod && SecondBulletMod->IsA(UBulletMagneticComp::StaticClass()))
		{
			SecondBulletMod->HandleMovement();
		}
		
	}
}

void ABullet::InitializeVariables(float BulletDamage, float BulletVelocity)
{
	Damage = BulletDamage;
	ProjectileMovement->InitialSpeed = BulletVelocity;
	ProjectileMovement->MaxSpeed = BulletVelocity;
	isGotBulletMod = false;
}

void ABullet::InitializeVariables(float BulletDamage, float BulletVelocity, int BulletModIndex, int SecondBulletModIndex)
{
	Damage = BulletDamage;
	ProjectileMovement->InitialSpeed = BulletVelocity;
	ProjectileMovement->MaxSpeed = BulletVelocity;
	if(BulletModIndex != -1)
	{
		BulletMod = Cast<UBulletBaseComponent>(AddComponentByClass(BulletComps[BulletModIndex] ,false, FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(1.0, 1.0, 1.0)), false ));
		isGotBulletMod = true;
	}
	if(SecondBulletModIndex != -1)
	{
		SecondBulletMod = Cast<UBulletBaseComponent>(AddComponentByClass(BulletComps[SecondBulletModIndex] ,false, FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector(1.0, 1.0, 1.0)), false ));
		isGotBulletMod = true;
	}
	
}
	
	
	

void ABullet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABullet, Damage);
	DOREPLIFETIME(ABullet, BulletMod);
}

void ABullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if(HasAuthority())
	{
		if(isGotBulletMod && !SecondBulletMod)
		{
			if(BulletMod->HandleImpact(Hit))
			{
				 Destroy();
			}
			
		}
		else if(isGotBulletMod && SecondBulletMod)
		{
			if(BulletMod->HandleImpact(Hit) && SecondBulletMod->HandleImpact(Hit))
			{
				Destroy();
			}
		}
		else
		{
			DoDamage(Other, Hit);
			Destroy();
		}
		
		
	}
}

void ABullet::DoDamage(AActor* Other , const FHitResult& Hit)
{
	if(auto temp = Cast<IDamageable>(Other))
	{
		temp->DetailedTakeDamage2(Damage, Hit.Location, Hit.BoneName);
		UE_LOG(LogTemp, Warning, TEXT("Has Been Hit : %s"), *Hit.BoneName.ToString());
	}
}

