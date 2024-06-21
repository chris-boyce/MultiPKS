
#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "Explosion.h"
#include "BulletExplosive.generated.h"

/**
* ABulletExplosive class defines the behavior of exploding bullets in the game.
* Inherits from ABullet and adds functionality for explosions on impact.
 */
UCLASS()
class MULTIPKS_API ABulletExplosive : public ABullet
{
	GENERATED_BODY()
public:
	ABulletExplosive();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Handler for when a bullet hits another object
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AExplosion> ExplosiveClass;
protected:


	// Function to handle the explosion effects and damage application
	void Explode(const FVector& HitLocation);
	
};
