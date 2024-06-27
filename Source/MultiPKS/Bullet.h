// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBaseComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class MULTIPKS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void InitializeVariables(float BulletDamage, float BulletVelocity);

	void InitializeVariables(float BulletDamage, float BulletVelocity, int BulletModIndex, int SecondBulletModIndex);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	float Damage = 0;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;


	void DoDamage(AActor* Other, const FHitResult& Hit);
private:

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UBulletBaseComponent>> BulletComps;
	
	UPROPERTY(Replicated, VisibleAnywhere, Category="BulletMod")
	UBulletBaseComponent* BulletMod;

	UPROPERTY(Replicated, VisibleAnywhere, Category="BulletMod")
	UBulletBaseComponent* SecondBulletMod;

	UPROPERTY(VisibleAnywhere, Category="BulletMod")
	bool isGotBulletMod = false;
	
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	

	

};
