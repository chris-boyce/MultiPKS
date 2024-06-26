// Fill out your copyright notice in the Description page of Project Settings.


#include "Grip.h"

#include "Net/UnrealNetwork.h"


AGrip::AGrip()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	UnADSMoveSpeedRange = FVector2D(400.0f, 800.0f);
	ADSedMoveSpeedRange =  FVector2D(100.0f, 600.0f);
	RecoilAmountRange =  FVector2D(0.1f, 3.0f);
}

void AGrip::BeginPlay()
{
	Super::BeginPlay();
}

void AGrip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGrip, AttachmentValue);
	DOREPLIFETIME(AGrip, UnADSMoveSpeed);
	DOREPLIFETIME(AGrip, ADSedMoveSpeed);
	DOREPLIFETIME(AGrip, RecoilAmount);
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

void AGrip::AdjustScaleValue(int Scale)
{
	float ScaleAlpha = static_cast<float>(Scale) / 100.0f;
	AttachmentValue = Scale;
	UnADSMoveSpeed = FMath::Lerp(UnADSMoveSpeedRange.X, UnADSMoveSpeedRange.Y, ScaleAlpha);
	ADSedMoveSpeed = FMath::Lerp(ADSedMoveSpeedRange.X, ADSedMoveSpeedRange.Y, ScaleAlpha);
	RecoilAmount = FMath::Lerp(RecoilAmountRange.X, RecoilAmountRange.Y, ScaleAlpha);
}

