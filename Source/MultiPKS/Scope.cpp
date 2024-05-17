// Fill out your copyright notice in the Description page of Project Settings.


#include "Scope.h"

#include "Net/UnrealNetwork.h"

AScope::AScope()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AScope::BeginPlay()
{
	Super::BeginPlay();
	
}

void AScope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScope::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AScope, ADSSpeed);
	DOREPLIFETIME(AScope, FOVChange);
	
	
}

