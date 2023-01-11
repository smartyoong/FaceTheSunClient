// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerSciFiAnimation.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UPlayerSciFiAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Speed") float speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Direction") float Direction = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jump") bool bIsInAir = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Jump") bool bIsCrouch = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pitch") double PlayerPitch;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION()
	void AnimNotify_EndReload();
	UFUNCTION()
	void AnimNotify_EndReload1P();
};
