// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UCommonEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void AnimNotify_Attack();
	UFUNCTION()
	void AnimNotify_Die();
	
};
