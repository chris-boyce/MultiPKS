// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class MULTIPKS_API ABaseEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	ABaseEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDamage(float DamageAmount) override;

	virtual float GetHealth() const override;
	
	UActorComponent* GetComponentByName(FName CompName);
	
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    FVector NormalImpulse, const FHitResult& Hit);


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, VisibleAnywhere)
	float CurrentHealth = 100.0f;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void DamageText(float Amount, FVector HitLocation);

};
