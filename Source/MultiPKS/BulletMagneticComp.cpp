// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletMagneticComp.h"

#include "BaseEnemy.h"
#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

UBulletMagneticComp::UBulletMagneticComp()
{
	
}

void UBulletMagneticComp::BeginPlay()
{
	Super::BeginPlay();
	if (USceneComponent* RootComp = Cast<USceneComponent>(GetOwner()->GetRootComponent()))
	{
		DetectionSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("DetectionSphere"));
		if (DetectionSphere)
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Created Detection"));
			DetectionSphere->InitSphereRadius(500.0f);  
			DetectionSphere->SetCollisionProfileName(TEXT("Sensor")); 
			DetectionSphere->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
			DetectionSphere->RegisterComponent();  
			DetectionSphere->SetGenerateOverlapEvents(true);

			DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &UBulletMagneticComp::OnOverlapBegin);
		}
	}
}

void UBulletMagneticComp::HandleMovement()
{
	Super::HandleMovement();
	if (CurrentTarget)
	{
		FVector Direction = (CurrentTarget->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
		UProjectileMovementComponent* ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
		float CurrentSpeed = ProjectileMovement->Velocity.Size();  
		
		if (ProjectileMovement)
		{
			ProjectileMovement->Velocity = Direction * CurrentSpeed;
		}
	}
}

void UBulletMagneticComp::HandleImpact(const FHitResult& HitResult)
{
	Super::HandleImpact(HitResult);
	auto temp = Cast<ABullet>(GetOwner());
	temp->DoDamage(HitResult.GetActor(), HitResult);
}

void UBulletMagneticComp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Has  Dected Overlap"));
	if (IsValidTarget(OtherActor))
	{
		CurrentTarget = OtherActor;  // Update your target
	}
	
}

bool UBulletMagneticComp::IsValidTarget(AActor* OtherActor) const
{
	if (OtherActor && OtherActor->IsA(ABaseEnemy::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Is Valid Target"));
		return true;
	}
	return false;
}