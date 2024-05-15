// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"


UPoseSearchDatabase* UPlayerAnimInstance::ChoosePoseDatabase(bool isFalling, bool isCrouch, bool isArmed)
{
	if(isFalling && !isArmed)
	{
		return UnarmedJump;
	}
	if(isCrouch && !isArmed)
	{
		return UnarmedCrouch;
	}
	if(!isCrouch && !isFalling)
	{
		return UnarmedWalk;
	}
	return nullptr;
	
}
