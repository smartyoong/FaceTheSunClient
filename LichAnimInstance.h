// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnemyAnimInstance.h"
#include "LichAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ULichAnimInstance : public UCommonEnemyAnimInstance
{
	GENERATED_BODY()
	public:
	UFUNCTION()
	void AnimNotify_LichSkill();
	UFUNCTION()
	void AnimNotify_LichDeath();
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface * DecalMaterial;
};
