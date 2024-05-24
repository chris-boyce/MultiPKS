// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"
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
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation , SpawnRotation , ActorSpawnParams);

	FiringCharacter->RotateCamera(Grip->RecoilAmount, 0.0f);
}

