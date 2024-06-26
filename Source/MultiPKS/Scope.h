// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThirdPersonCharacter.h"
#include "GameFramework/Actor.h"
#include "Scope.generated.h"

UCLASS()
class MULTIPKS_API AScope : public AActor
{
	GENERATED_BODY()
	
public:	
	AScope();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Scope")
	FString AttachmentName = "Scope";

	UPROPERTY(Replicated, VisibleAnywhere, Category="Scope")
	int AttachmentValue = 0;

	UPROPERTY(EditDefaultsOnly, Category="Scope|ADS")
	FVector2D ADSSpeedRange;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category="Scope|ADS")
	float ADSSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ScopeWidgetClass;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* ScopeWidget;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Scope|ADS")
	float FOVChange = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Seed")
	FString Seed = "A";

	UFUNCTION()
	void ToggleScopeVisual(AThirdPersonCharacter* PlayerCharacter, bool isADSed);

	UFUNCTION()
	void ToggleCameraPosition(UCameraComponent* PlayerCamera, bool isADSed);
	
	//UFUNCTION(NetMulticast,Reliable)
	//void Multi_ToggleCameraPosition(UCameraComponent* PlayerCamera, bool isADSed);

	UFUNCTION()
	void AdjustScaleValue(int Scale);
};
