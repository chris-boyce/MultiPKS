// Fill out your copyright notice in the Description page of Project Settings.


#include "GunDataSingleton.h"

UGunDataSingleton* UGunDataSingleton::SingletonInstance = nullptr;

UGunDataSingleton* UGunDataSingleton::Get()
{
	if (!SingletonInstance)
	{
		SingletonInstance = NewObject<UGunDataSingleton>();
		SingletonInstance->AddToRoot();  // Prevent garbage collection
	}
	return SingletonInstance;
}
