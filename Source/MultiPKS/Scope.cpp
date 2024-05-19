// Fill out your copyright notice in the Description page of Project Settings.


#include "Scope.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

AScope::AScope()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

}

void AScope::BeginPlay()
{
	Super::BeginPlay();
	
}

void AScope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScope::ToggleScopeVisual(AThirdPersonCharacter* PlayerCharacter, bool isADSed)
{
	UE_LOG(LogTemp, Error, TEXT("ADS HAS COMPLETED !!! + ++ + ++ "));
	if(!ScopeWidget)
	{
		APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
		ScopeWidget = CreateWidget<UUserWidget>(PC, ScopeWidgetClass);
	}
	
	if(isADSed)
	{
		ScopeWidget->AddToViewport();
	}
	else
	{
		ScopeWidget->RemoveFromParent();
		
	}
}

void AScope::ToggleCameraPosition(UCameraComponent* PlayerCamera, bool isADSed)
{
	UE_LOG(LogTemp, Warning, TEXT("--------- SERVER TOGGLE CAMERA ----------"));
	UE_LOG(LogTemp, Warning, TEXT("--------- TOGGLE CAMERA POS HAS RUN ----------"));
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	if(isADSed)
	{
		UKismetSystemLibrary::MoveComponentTo(PlayerCamera, FVector(10.0,55,80),FRotator(0,0,0), false, true, ADSSpeed,true,EMoveComponentAction::Move, LatentInfo);
		PlayerCamera->SetFieldOfView(90);
	}
	else
	{
		UKismetSystemLibrary::MoveComponentTo(PlayerCamera, FVector(400,0,60),FRotator(0,0,0), false, false, ADSSpeed,true,EMoveComponentAction::Move, LatentInfo);
		PlayerCamera->SetFieldOfView(FOVChange);
	}
	
	/*
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("--------- TOGGLE CAMERA POS ----------"));
		Multi_ToggleCameraPosition(PlayerCamera, isADSed);
		
	}
	else
	{
		Server_ToggleCameraPosition(PlayerCamera, isADSed);
	}
	*/
	

}

void AScope::Multi_ToggleCameraPosition_Implementation(UCameraComponent* PlayerCamera, bool isADSed)
{
	UE_LOG(LogTemp, Warning, TEXT("--------- TOGGLE CAMERA POS HAS RUN ----------"));
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	if(isADSed)
	{
		UKismetSystemLibrary::MoveComponentTo(PlayerCamera, FVector(10.0,55,80),FRotator(0,0,0), false, true, ADSSpeed,true,EMoveComponentAction::Move, LatentInfo);
		PlayerCamera->SetFieldOfView(90);
	}
	else
	{
		UKismetSystemLibrary::MoveComponentTo(PlayerCamera, FVector(400,0,60),FRotator(0,0,0), false, false, ADSSpeed,true,EMoveComponentAction::Move, LatentInfo);
		PlayerCamera->SetFieldOfView(FOVChange);
	}
}

void AScope::Server_ToggleCameraPosition_Implementation(UCameraComponent* PlayerCamera, bool isADSed)
{
	
}


void AScope::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AScope, ADSSpeed);
	DOREPLIFETIME(AScope, FOVChange);
	
	
}

