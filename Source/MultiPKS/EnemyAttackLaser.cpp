// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackLaser.h"

#include "NiagaraComponent.h"
#include "Damageable.h" // Your IDamageable interface header
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

void UEnemyAttackLaser::BeginPlay()
{
    Super::BeginPlay();
    if (GetOwner()->HasAuthority()) // Check if we are the server
    {
        LaserParticle = Cast<UNiagaraComponent>(GetOwner()->GetComponentByClass(UNiagaraComponent::StaticClass()));
        LaserParticle->Deactivate();
    }
}

void UEnemyAttackLaser::BeginAttack()
{
    if (!GetOwner()->HasAuthority()) return; // Only execute on the server

    FVector Start = GetOwner()->GetActorLocation(); 
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector End = Start + (ForwardVector * 1500.0f); 
    
    FHitResult OutHit;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner()); 
    
    bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Camera, CollisionParams);
    

    FVector RandomOffset = FVector(FMath::RandRange(-50.f, 50.f), FMath::RandRange(-50.f, 50.f), FMath::RandRange(-5.0f, 5.0f));
    FVector RandomEnd = bHit ? OutHit.Location + RandomOffset : End; 
    
    MulticastStartLaserEffect(Start, RandomEnd); // Start the effect on all clients

    if (bHit)
    {
        IDamageable* DamageableActor = Cast<IDamageable>(OutHit.GetActor());
        if (DamageableActor)
        {
            DamageableActor->DetailedTakeDamage2(10, RandomEnd, OutHit.BoneName);
        }
    }
}

void UEnemyAttackLaser::MulticastStartLaserEffect_Implementation(FVector NewBeamStart, FVector NewBeamEnd)
{
    if (!LaserParticle) return;
    this->BeamStart = NewBeamStart; // assign to member variable
    this->BeamEnd = NewBeamEnd;     // assign to member variable
    LaserParticle->SetNiagaraVariableVec3("User.BeamStart", NewBeamStart); // set to particle system
    LaserParticle->SetNiagaraVariableVec3("User.BeamEnd", NewBeamEnd);
    LaserParticle->Activate();


    FTimerHandle TimerHandle_LaserOff;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle_LaserOff, FTimerDelegate::CreateLambda([this]() {
        LaserParticle->Deactivate();
    }), 1.0f, false);
}


void UEnemyAttackLaser::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UEnemyAttackLaser, LaserParticle);
    DOREPLIFETIME_CONDITION(UEnemyAttackLaser, BeamStart, COND_SkipOwner); 
    DOREPLIFETIME_CONDITION(UEnemyAttackLaser, BeamEnd, COND_SkipOwner);   
}
