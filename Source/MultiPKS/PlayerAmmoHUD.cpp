// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAmmoHUD.h"

#include "Components/TextBlock.h"

void UPlayerAmmoHUD::SetAmmoText(int CurrentAmmo, int MaxAmmo)
{
	TXT_CurrentAmmo->SetText(FText::AsNumber(CurrentAmmo));
	TXT_MaxAmmo->SetText(FText::AsNumber(MaxAmmo));
}
