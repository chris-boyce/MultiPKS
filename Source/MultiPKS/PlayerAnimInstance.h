// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PoseSearch/PoseSearchDatabase.h"
#include "PlayerAnimInstance.generated.h"


UCLASS()
class MULTIPKS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UPoseSearchDatabase* ChoosePoseDatabase(bool isFalling, bool isCrouch, bool isArmed, bool isSliding);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PoseDataBase")
	UPoseSearchDatabase* UnarmedCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PoseDataBase")
	UPoseSearchDatabase* UnarmedJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PoseDataBase")
	UPoseSearchDatabase* UnarmedWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PoseDataBase")
	UPoseSearchDatabase* ArmedWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PoseDataBase")
	UPoseSearchDatabase* ArmedJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PoseDataBase")
	UPoseSearchDatabase* ArmedCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PoseDataBase")
	UPoseSearchDatabase* UnarmedSlide;
	
	
};
