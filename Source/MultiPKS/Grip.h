// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultiPKS/ThirdPersonCharacter.h"
#include "Grip.generated.h"

UCLASS()
class MULTIPKS_API AGrip : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrip();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category="Grip")
	FString AttachmentName = "Grip";

	UPROPERTY(Replicated, VisibleAnywhere, Category="Grip")
	int AttachmentValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Grip|Attribute")
	FVector2D UnADSMoveSpeedRange;

	UPROPERTY(Replicated,EditDefaultsOnly, Category="Grip|Attribute")
	float UnADSMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Grip|Attribute")
	FVector2D ADSedMoveSpeedRange;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Grip|Attribute")
	float ADSedMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category="Grip|Attribute")
	FVector2D RecoilAmountRange;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Grip|Attribute")
	float RecoilAmount;

	UFUNCTION()
	void ChangePlayerSpeed(AThirdPersonCharacter* Player, bool isADSed);

	UFUNCTION()
	void AdjustScaleValue(int Scale);
	

	

};
