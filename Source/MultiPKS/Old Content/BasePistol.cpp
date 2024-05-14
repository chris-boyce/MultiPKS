// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePistol.h"

#include "Kismet/GameplayStatics.h"
#include "MultiPKS/ThirdPersonCharacter.h"


ABasePistol::ABasePistol()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ABasePistol::BeginPlay()
{
	Super::BeginPlay();
	if (MagazineClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		MagazineComponent = GetWorld()->SpawnActor<AMagazine>(MagazineClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		if (MagazineComponent)
		{
			MagazineComponent->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	
}

void ABasePistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePistol::HighlightObject()
{
	EnableOutline();
}

void ABasePistol::UnHighlightObject()
{
	DisableOutline();
}

ABasePistol* ABasePistol::PickupObject(AThirdPersonCharacter* InteractingCharacter)
{
	return this;
}

void ABasePistol::FireDown(AThirdPersonCharacter* Char)
{
	SetOwner(Char);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetName());
	Fire(Char);
	//GetWorld()->GetTimerManager().SetTimer(FiringTimerHandle, this, &ABasePistol::Fire, 0.2f, true);
}

void ABasePistol::FireUp()
{
	//GetWorld()->GetTimerManager().ClearTimer(FiringTimerHandle);
}

void ABasePistol::Fire(AThirdPersonCharacter* FiringCharacter)
{
	SetOwner(FiringCharacter);
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Proj Class"));
		return;
	}
	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No world"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Passed Fire Function Tests"));

	APlayerController* PlayerController = Cast<APlayerController>(FiringCharacter->GetController());
	FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector SpawnLocation = FiringCharacter->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation , SpawnRotation , ActorSpawnParams);
	
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, FiringCharacter->GetActorLocation());
	}
	
	
}






