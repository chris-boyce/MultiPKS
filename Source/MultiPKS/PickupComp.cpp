// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComp.h"

#include "BasePistol.h"
#include "MultiPKSCharacter.h"
#include "Components/SphereComponent.h"

UPickupComp::UPickupComp()
{
	
}

void UPickupComp::BeginPlay()
{
	Super::BeginPlay();

}

void UPickupComp::AttachGun(AMultiPKSCharacter* Character)
{
	if (Character->HasAuthority())
	{
		Multi_AttachGun(Character);
	}
	else
	{
		Server_AttachGun(Character);
	}
}

void UPickupComp::Server_AttachGun_Implementation(AMultiPKSCharacter* Character)
{
	Multi_AttachGun(Character);
}

void UPickupComp::Multi_AttachGun_Implementation(AMultiPKSCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Called Multi"));
	//FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	//GetOwner()->AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	//Character->AddInstanceComponent(this);
	//USphereComponent* SphereComp = Cast<USphereComponent>(GetOwner()->GetComponentByClass(USphereComponent::StaticClass()));
	//if (SphereComp)
	//{
		//SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
	//Cast<ABasePistol>(GetOwner())->Character = Character;

	if(Cast<ABasePistol>(GetOwner()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Called Add Weapon To Inventory"));
		Character->WeaponInventory->AddWeaponToInventory(Cast<ABasePistol>(GetOwner()));
		
	}
	
}

bool UPickupComp::Multi_AttachGun_Validate(AMultiPKSCharacter* Character)
{
	return true;
}

bool UPickupComp::Server_AttachGun_Validate(AMultiPKSCharacter* Character)
{
	return true;
}






