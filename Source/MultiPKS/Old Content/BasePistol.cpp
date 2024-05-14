// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePistol.h"

#include "Kismet/GameplayStatics.h"
#include "MultiPKS/ThirdPersonCharacter.h"


ABasePistol::ABasePistol()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABasePistol::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (MagazineClasses.Num() > 0 && EditMode == false)
	{
		int32 RandomIndex = FMath::RandRange(0, MagazineClasses.Num() - 1);
		TSubclassOf<AMagazine> SelectedMagazineClass = MagazineClasses[RandomIndex];
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		if(MagazineComponent)
		{
			MagazineComponent->Destroy();
		}
		MagazineComponent = GetWorld()->SpawnActor<AMagazine>(SelectedMagazineClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		MagazineComponent->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}


void ABasePistol::BeginPlay()
{
	Super::BeginPlay();
	
	
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
}

void ABasePistol::FireUp()
{

}

void ABasePistol::Fire(AThirdPersonCharacter* FiringCharacter)
{
	SetOwner(FiringCharacter);
	if (ProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Proj Class"));
		return;
	}
	if(MagazineComponent->CurrentAmmo <= 0)
	{
		MagazineComponent->ReloadMag();
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Passed Fire Function Tests"));

	MagazineComponent->ConsumeAmmo();

	UE_LOG(LogTemp, Log, TEXT("Current Ammo: %d"), MagazineComponent->CurrentAmmo);
	

	APlayerController* PlayerController = Cast<APlayerController>(FiringCharacter->GetController());
	FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector SpawnLocation = FiringCharacter->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation , SpawnRotation , ActorSpawnParams);
	Multi_FireSound(SpawnLocation);
	
	
	
}

void ABasePistol::Multi_FireSound_Implementation(FVector Location)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered Sound Function"));
	if (FireSound != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing Sound"));
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Location);
	}
}

bool ABasePistol::Multi_FireSound_Validate(FVector Location)
{
	return true;
}








