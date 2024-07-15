// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackLaser.h"

#include "Damageable.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"




void UEnemyAttackLaser::BeginPlay()
{
	Super::BeginPlay();
	LaserParticle = Cast<UNiagaraComponent>(GetOwner()->GetComponentByClass(UNiagaraComponent::StaticClass()));
	LaserParticle->Deactivate();
}

void UEnemyAttackLaser::BeginAttack()
{
	FVector Start = GetOwner()->GetActorLocation(); 
	FVector ForwardVector = GetOwner()->GetActorForwardVector();
	FVector End = Start + (ForwardVector * 1500.0f); 
	
	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner()); 
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Camera, CollisionParams);
	
	LaserParticle->Activate();
	
	FVector RandomOffset = FVector(FMath::RandRange(-50.f, 50.f), FMath::RandRange(-50.f, 50.f), FMath::RandRange(-5.0f, 5.0f));
	FVector RandomEnd = OutHit.Location + RandomOffset;

	LaserParticle->SetNiagaraVariableVec3("User.BeamEnd", RandomEnd);

	if (bHit)
	{
		LaserParticle->SetNiagaraVariableVec3("User.BeamEnd", RandomEnd);
		IDamageable* DamageableActor = Cast<IDamageable>(OutHit.GetActor());
		if (DamageableActor)
		{
			DamageableActor->DetailedTakeDamage2(10, RandomEnd, OutHit.BoneName);
		}
	}
	
	//DrawDebugLine(GetWorld(), Start, RandomEnd, FColor::Cyan, false, 1, 0, 5);

	FTimerHandle TimerHandle_LaserOff;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_LaserOff, FTimerDelegate::CreateLambda([this]() {
	 
		LaserParticle->Deactivate();
	
	}), 1.0f, false);
}

void UEnemyAttackLaser::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEnemyAttackLaser, LaserParticle);
	DOREPLIFETIME(UEnemyAttackLaser, start);
	DOREPLIFETIME(UEnemyAttackLaser, LaserParticle);
	
}
