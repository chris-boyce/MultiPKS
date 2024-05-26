// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCharacter.h"

#include "GameFramework/Actor.h"
#include "Barrel.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	Semi     UMETA(DisplayName = "Semi"),
	Automatic UMETA(DisplayName = "Automatic"),
	Burst    UMETA(DisplayName = "Burst")
};

UCLASS()
class MULTIPKS_API ABarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	ABarrel();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Barrel|Attributes")
	float FireRate = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category="Barrel|Attributes")
	float BulletDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category="Seed")
	FString Seed = "A";

	UPROPERTY(EditDefaultsOnly, Category="Barrel|Attributes")
	EFireMode FireMode = EFireMode::Automatic;

	UPROPERTY(EditDefaultsOnly, Category="Barrel|Attributes|Burst", meta = (EditCondition = "FireMode == EFireMode::Burst"))
	float BurstSpeed = 0.05f;

	UFUNCTION()
	void Fire(AThirdPersonCharacter* FiringCharacter, TSubclassOf<AActor> ProjectileClass, AMagazine* Magazine, AGrip* Grip, AMuzzle* Muzzle);

	UFUNCTION()
	void HandleFire(AThirdPersonCharacter* FiringCharacter, TSubclassOf<AActor> ProjectileClass, AMagazine* Magazine, AGrip* Grip, AMuzzle* Muzzle);

	FTimerHandle BurstFireTimerHandles[10];

	UPROPERTY(EditDefaultsOnly, Category="Barrel|Attributes|Burst", meta = (EditCondition = "FireMode == EFireMode::Burst"))
	int BurstCount = 3;
	

};
