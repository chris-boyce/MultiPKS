// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrel.h"

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

void ABarrel::Fire(AThirdPersonCharacter* FiringCharacter, TSubclassOf<AActor> ProjectileClass, AMagazine* Magazine)
{
	HandleFire(FiringCharacter, ProjectileClass, Magazine);

	if (FireMode == EFireMode::Burst)
	{
		for (int32 i = 1; i < BurstCount; i++)
		{
			GetWorld()->GetTimerManager().SetTimer(BurstFireTimerHandles[i], FTimerDelegate::CreateLambda([this, FiringCharacter, ProjectileClass, Magazine]()
			{
				HandleFire(FiringCharacter, ProjectileClass, Magazine);
			}), BurstSpeed * i, false);
		}
	}
}

void ABarrel::HandleFire(AThirdPersonCharacter* FiringCharacter, TSubclassOf<AActor> ProjectileClass, AMagazine* Magazine)
{
	if(!Magazine->CanFire())
	{
		FiringCharacter->Client_CallUpdateAmmo(Magazine->CurrentAmmo, Magazine->MaxAmmo);
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Passed Fire Function Tests"));

	Magazine->ConsumeAmmo();
	
	FiringCharacter->Client_CallUpdateAmmo(Magazine->CurrentAmmo, Magazine->MaxAmmo);
	FiringCharacter->RotateCamera(1.0f, 0.0f);
	
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
		FTransform ActorTransform = FiringCharacter->GetActorTransform();
		FVector WorldEditableVector = ActorTransform.TransformVector(FVector(100, 0, 50));
		SpawnLocation = FiringCharacter->GetActorLocation() + WorldEditableVector;
	}
	

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation , SpawnRotation , ActorSpawnParams);
}

