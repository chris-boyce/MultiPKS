// Fill out your copyright notice in the Description page of Project Settings.


#include "Muzzle.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMuzzle::AMuzzle()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

void AMuzzle::BeginPlay()
{
	Super::BeginPlay();
	
}



void AMuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMuzzle::PlayFireSound()
{
	if (HasAuthority())  
	{
		Multi_PlayFireSound();
	}
}

void AMuzzle::Multi_PlayFireSound_Implementation()
{
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound.Sound , GetActorLocation());
	
}

