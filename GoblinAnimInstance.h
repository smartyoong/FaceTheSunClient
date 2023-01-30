// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonEnemyAnimInstance.h"
#include "GoblinAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UGoblinAnimInstance : public UCommonEnemyAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AnimNotify_GoblinAttack();
	UFUNCTION()
	void AnimNotify_GoblinDie();	
};
