// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BowMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UBowMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackAnimation;
};
