// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BowAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UBowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AnimNotify_ArrowAttack();
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ASkeletalBowProjectile> ProjectileClass;
};
