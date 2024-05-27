// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "Bullet.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsAsset.h"


ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}


void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	auto SkeletalMeshComponent = GetMesh();
	
	SkeletalMeshComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	SkeletalMeshComponent->SetNotifyRigidBodyCollision(true);
	SkeletalMeshComponent->OnComponentHit.AddDynamic(this, &ABaseEnemy::OnComponentHit);

	
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::TakeDamage(float DamageAmount)
{
	CurrentHealth = CurrentHealth - DamageAmount;
	if(CurrentHealth <= 0)
	{
		Destroy();
	}
}

float ABaseEnemy::GetHealth() const
{
	return CurrentHealth;
}

UActorComponent* ABaseEnemy::GetComponentByName(FName CompName)
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Component : Components)
	{
		if (Component && Component->GetFName() == CompName)
		{
			return Component;
		}
	}
	return nullptr;
}

void ABaseEnemy::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
		//UE_LOG(LogTemp, Warning, TEXT("Has Been Hit : %s"), *Hit.MyBoneName.ToString());
}

void ABaseEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseEnemy, CurrentHealth);
}

void ABaseEnemy::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
                           FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	
}







