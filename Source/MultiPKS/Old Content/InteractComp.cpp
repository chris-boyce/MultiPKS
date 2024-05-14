// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComp.h"

#include "BasePistol.h"
#include "Components/SphereComponent.h"
#include "MultiPKS/Pickupable.h"
#include "MultiPKS/ThirdPersonCharacter.h"

UInteractComp::UInteractComp()
{
	
}

void UInteractComp::InteractWithObject()
{
	auto Controller = Cast<AThirdPersonCharacter>(GetOwner())->GetLocalViewingPlayerController();
	
	if (Controller)
	{
		FVector StartLocation;
		FRotator ViewRotation;
		Controller->GetPlayerViewPoint(StartLocation, ViewRotation);

		FVector EndLocation = StartLocation + (ViewRotation.Vector() * 500); // Trace 1000 units forward

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

		if (bHit)
		{
			//DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);
			IPickupable* Pickupable = Cast<IPickupable>(HitResult.GetActor());
			if (Pickupable)
			{
				auto Gun = Pickupable->PickupObject(Cast<AThirdPersonCharacter>(GetOwner()));
				if(Gun)
				{
					UE_LOG(LogTemp, Warning, TEXT("Has Got Reference To Gun"));
					
					if(GetOwner()->HasAuthority())
					{
						Multi_AttachGun(Gun);
					}
					else
					{
						Server_AttachGun(Gun);
					}
					
				}
			}
		}
	}
}

void UInteractComp::Server_AttachGun_Implementation(ABasePistol* Gun)
{
	Multi_AttachGun(Gun);
}

bool UInteractComp::Server_AttachGun_Validate(ABasePistol* Gun)
{
	return true;
}


void UInteractComp::Multi_AttachGun_Implementation(ABasePistol* Gun)
{
	Gun->AttachToComponent(Cast<AThirdPersonCharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GunSocket"));
	Gun->SphereComponentZ->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	auto Owner = Cast<AThirdPersonCharacter>(GetOwner());
	if(Owner->PlayerWeapon.Num() <= 1)
	{
		Owner->PlayerWeapon.Add(Gun);
	}
	else
	{
		DropWeapon.Broadcast(Cast<AThirdPersonCharacter>(GetOwner()));
		UE_LOG(LogTemp, Warning, TEXT("Fired BroadCast"));
		Owner->PlayerWeapon[Owner->CurrentlySelectedWeapon] = Gun;
	}
	
}

bool UInteractComp::Multi_AttachGun_Validate(ABasePistol* Gun)
{
	return true;
}

void UInteractComp::PerformLineTrace()
{
	auto Controller = Cast<AThirdPersonCharacter>(GetOwner())->GetLocalViewingPlayerController();
	
	if (Controller)
	{
		FVector StartLocation;
		FRotator ViewRotation;
		Controller->GetPlayerViewPoint(StartLocation, ViewRotation);

		FVector EndLocation = StartLocation + (ViewRotation.Vector() * 500); // Trace 1000 units forward

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

		if (bHit)
		{
			//DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);
			IPickupable* Pickupable = Cast<IPickupable>(HitResult.GetActor());
			if (Pickupable)
			{
				Pickupable->HighlightObject();
			}
		}
	}
}


void UInteractComp::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInteractComp::PerformLineTrace, 0.2f, true);
	
}





