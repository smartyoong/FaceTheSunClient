// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnemy.h"
#include "LichEnemy.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ALichEnemy : public ACommonEnemy
{
	GENERATED_BODY()
public:
	UFUNCTION(NetMulticast,Reliable,BlueprintCallable)
	void MultiLichAttack();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MultiLichAttackReal();
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SkillAttackAnim;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ALichSkillActor> LichSkillClass;
	FVector SkillLocation;
	FRotator SkillRotator;
};
