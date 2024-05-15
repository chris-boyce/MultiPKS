// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickupable.generated.h"

class AThirdPersonCharacter;
class ABasePistol;

UINTERFACE(MinimalAPI)
class UPickupable : public UInterface
{
	GENERATED_BODY()
};


class MULTIPKS_API IPickupable
{
	GENERATED_BODY()

public:
	virtual void HighlightObject(AThirdPersonCharacter* InteractingCharacter) = 0;

	virtual void UnHighlightObject(AThirdPersonCharacter* InteractingCharacter) = 0;

	virtual ABasePistol* PickupObject(AThirdPersonCharacter* Character) = 0;
	
};
