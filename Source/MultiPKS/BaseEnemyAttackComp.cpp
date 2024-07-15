// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAttackComp.h"

UBaseEnemyAttackComp::UBaseEnemyAttackComp()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UBaseEnemyAttackComp::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBaseEnemyAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBaseEnemyAttackComp::BeginAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Base Attack Called"));
}

