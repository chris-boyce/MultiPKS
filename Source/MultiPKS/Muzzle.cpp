// Fill out your copyright notice in the Description page of Project Settings.


#include "Muzzle.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMuzzle::AMuzzle()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	bReplicates = true;

}

void AMuzzle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMuzzle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMuzzle, AttachmentValue);
	DOREPLIFETIME(AMuzzle, BulletVelocity);
	DOREPLIFETIME(AMuzzle, FireSound);
	
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

void AMuzzle::AdjustScaleValue(int Scale)
{
	AttachmentValue = Scale;
	int ReverseScale = 99 - Scale;
	BulletVelocity = BulletVelocity * (static_cast<float>(ReverseScale) / 100.0f);
}

void AMuzzle::Multi_PlayFireSound_Implementation()
{
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound.Sound , GetActorLocation());
	
}

