// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Muzzle.generated.h"

UENUM(BlueprintType)
enum class ESoundLevel : uint8
{
	Loud UMETA(DisplayName = "Loud"),
	Medium UMETA(DisplayName = "Medium"),
	Silenced UMETA(DisplayName = "Silenced")
};

USTRUCT(BlueprintType)
struct FBulletSound
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	ESoundLevel SoundLevel;

	float Volume;

	FBulletSound() : Sound(nullptr), SoundLevel(ESoundLevel::Medium), Volume(1){}
};

UCLASS()
class MULTIPKS_API AMuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	AMuzzle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Muzzle|Attribute")
	float BulletVelocity;

	UPROPERTY(EditDefaultsOnly, Category="Muzzle|Sound")
	FBulletSound FireSound;

	void PlayFireSound();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayFireSound();

	

};
