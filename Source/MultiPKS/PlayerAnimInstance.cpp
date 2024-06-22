// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"


UPoseSearchDatabase* UPlayerAnimInstance::ChoosePoseDatabase(bool isFalling, bool isCrouch, bool isArmed, bool isSliding)
{
	if(isSliding)
	{
		return UnarmedSlide;
	}
	if(isFalling && !isArmed) 
	{
		return UnarmedJump;
	}
	if(isCrouch && !isArmed) 
	{
		return UnarmedCrouch;
	}
	if(!isCrouch && !isFalling && !isArmed) 
	{
		return UnarmedWalk;
	}
	if(isArmed && !isCrouch && !isFalling)
	{
		return ArmedWalk;
	}
	if(isArmed && isCrouch && !isFalling)
	{
		return ArmedCrouch;
	}
	if(isArmed && !isCrouch && isFalling)
	{
		return ArmedJump;
	}
	return nullptr;
	
}
