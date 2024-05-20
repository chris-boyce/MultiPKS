// Fill out your copyright notice in the Description page of Project Settings.


#include "Grip.h"


AGrip::AGrip()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
}

void AGrip::BeginPlay()
{
	Super::BeginPlay();
}

void AGrip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrip::ChangePlayerSpeed(AThirdPersonCharacter* Player, bool isADSed)
{
	if(isADSed)
	{
		Player->ChangeMoveSpeed(UnADSMoveSpeed);
	}
	else
	{
		Player->ChangeMoveSpeed(ADSedMoveSpeed);
	}
	
}

