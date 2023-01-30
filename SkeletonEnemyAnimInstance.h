// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnemyAnimInstance.h"
#include "SkeletonEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API USkeletonEnemyAnimInstance : public UCommonEnemyAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void AnimNotify_BowAttack();
};
