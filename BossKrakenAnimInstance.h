// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnemyAnimInstance.h"
#include "BossKrakenAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UBossKrakenAnimInstance : public UCommonEnemyAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Direction;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION()
	void AnimNotify_DieBoss();
	UFUNCTION()
	void AnimNotify_HealBoss();
	UFUNCTION()
	void AnimNotify_SmashBoss();
	UFUNCTION()
	void AnimNotify_BeamBoss();
	UFUNCTION()
	void AnimNotify_HomingBoss();
	UPROPERTY(EditDefaultsOnly, Category = Heal)
	class UParticleSystem* HealParticle;
	UPROPERTY(EditDefaultsOnly, Category = Heal)
	USoundBase* HealSound;
	class ABossKraken* Character;
	UPROPERTY(EditDefaultsOnly, Category=Skill)
	TSubclassOf<class AKrakenProjectile> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category=Skill)
	TSubclassOf<class AKrakenBeamSkillActor> BeamClass;
	UPROPERTY(EditDefaultsOnly, Category=Skill)
	TSubclassOf<class AKrakenSmashActor> Smashclass;
};
