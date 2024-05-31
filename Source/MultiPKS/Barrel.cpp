// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"

#include "Bullet.h"
#include "ThirdPersonCharacter.h"
#include "Grip.h"
#include "Muzzle.h"
#include "Camera/CameraComponent.h"


ABarrel::ABarrel()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	FireRateRange = FVector2D(1.0f, 1.1f);
	DamageRange = FVector2D(1.0f, 1.1f);
	BurstSpeedRange = FVector2D(1.0f, 1.1f);

}

void ABarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABarrel::Fire(AThirdPersonCharacter* FiringCharacter, TSubclassOf<AActor> ProjectileClass, AMagazine* Magazine, AGrip* Grip, AMuzzle* Muzzle)
{
	HandleFire(FiringCharacter, ProjectileClass, Magazine, Grip, Muzzle);

	if (FireMode == EFireMode::Burst)
	{
		for (int32 i = 1; i < BurstCount; i++)
		{
			GetWorld()->GetTimerManager().SetTimer(BurstFireTimerHandles[i], FTimerDelegate::CreateLambda([this, FiringCharacter, ProjectileClass, Magazine, Grip, Muzzle]()
			{
				HandleFire(FiringCharacter, ProjectileClass, Magazine, Grip, Muzzle);
			}), BurstSpeed * i, false);
		}
	}
}

void ABarrel::HandleFire(AThirdPersonCharacter* FiringCharacter, TSubclassOf<AActor> ProjectileClass, AMagazine* Magazine, AGrip* Grip, AMuzzle* Muzzle)
{
	if(!Magazine->CanFire())
	{
		FiringCharacter->Client_CallUpdateAmmo(Magazine->CurrentAmmo, Magazine->MaxAmmo);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Passed Fire Function Tests"));

	Magazine->ConsumeAmmo();
	Muzzle->PlayFireSound();
	FiringCharacter->Client_CallUpdateAmmo(Magazine->CurrentAmmo, Magazine->MaxAmmo);
	
	
	FRotator SpawnRotation;
	FVector SpawnLocation;
	if(!FiringCharacter->GetIsADS())
	{
		SpawnRotation = FiringCharacter->GetMainCameraComponent()->GetComponentRotation();
		SpawnLocation = FiringCharacter->GetMainCameraComponent()->GetComponentLocation();
	}
	else
	{
		SpawnRotation = FiringCharacter->GetMainCameraComponent()->GetComponentRotation();
		SpawnLocation = FiringCharacter->GetMainCameraComponent()->GetComponentLocation();
	}
	

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ProjectileClass, SpawnLocation , SpawnRotation , ActorSpawnParams);
	Bullet->InitializeVariables(BulletDamage, Muzzle->BulletVelocity);


	FiringCharacter->RotateCamera(Grip->RecoilAmount, 0.0f);
}

void ABarrel::AdjustScaleValue(int Scale)
{
	float ScaleAlpha = static_cast<float>(Scale) / 100.0f;
	
	FireRate = FMath::Lerp(FireRateRange.X, FireRateRange.Y, ScaleAlpha);
	BurstSpeed = FMath::Lerp(BurstSpeedRange.X, BurstSpeedRange.Y, ScaleAlpha);
	BulletDamage = FMath::Lerp(DamageRange.X, DamageRange.Y, ScaleAlpha);
}

