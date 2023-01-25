// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnemy.h"
#include "BossKraken.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ABossKraken : public ACommonEnemy
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated,BlueprintReadWrite)
	float StartHP;
	virtual void BeginPlay() override;
	UFUNCTION(NetMulticast, Reliable, Blueprintcallable)
	void MultiHPUnder30();
	UPROPERTY(EditDefaultsOnly)
	class USoundCue* CheerUpSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* InitStartSound;
	UFUNCTION(NetMulticast, Reliable, Blueprintcallable)
	void MultiHeal();
	UPROPERTY(EditDefaultsOnly)
	int32 HealHP = 1000;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HealAnim;
	class UAudioComponent* CurrentPlaySound;
	UFUNCTION(NetMulticast, Reliable, Blueprintcallable)
	void MultiStopBGMSound();
	UFUNCTION(NetMulticast, Reliable, Blueprintcallable)
	void MultiPlayBGMSound();
	ABossKraken();
	UFUNCTION(NetMulticast, Reliable, Blueprintcallable)
	void MultiBeam();
	UFUNCTION(NetMulticast, Reliable, Blueprintcallable)
	void MultiSmash();
	UFUNCTION(NetMulticast, Reliable, Blueprintcallable)
	void MultiProjectile();
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* BeamAnim;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* SmashAnim;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HomingAnim;
};
