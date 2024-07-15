// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackProjectile.h"

#include "Bullet.h"
#include "Net/UnrealNetwork.h"

void UEnemyAttackProjectile::BeginAttack()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		Multicast_SpawnBullet();
	}
	else 
	{
		
	}
}



void UEnemyAttackProjectile::Client_PredictAttack_Implementation()
{
	
}

void UEnemyAttackProjectile::Multicast_SpawnBullet_Implementation()
{
	if (!ProjectileClass) return;

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(
		ProjectileClass,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorForwardVector().Rotation(),
		ActorSpawnParams
	);

	Bullet->InitializeVariables(10, 800, -1, -1);
}

void UEnemyAttackProjectile::Server_BeginAttack_Implementation()
{
	
}

void UEnemyAttackProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


