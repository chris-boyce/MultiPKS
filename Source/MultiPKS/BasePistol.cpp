// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePistol.h"

#include "Kismet/GameplayStatics.h"


ABasePistol::ABasePistol()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ABasePistol::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABasePistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePistol::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
			
			if(!Character->HasAuthority())
			{
				Server_OnFire(SpawnLocation, SpawnRotation);
			}
			else
			{
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				
				World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
			
		}
	}
	
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	

	if (FireAnimation != nullptr)
	{

		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

bool ABasePistol::Server_OnFire_Validate(FVector Location, FRotator Rotation)
{
	return true;
}

void ABasePistol::Server_OnFire_Implementation(FVector Location, FRotator Rotation)
{
	UE_LOG(LogTemp, Warning, TEXT("Firing On Server"));
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, Location, Rotation, ActorSpawnParams);
}
