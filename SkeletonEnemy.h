// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnemy.h"
#include "SkeletonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ASkeletonEnemy : public ACommonEnemy
{
	GENERATED_BODY()
public:
	ASkeletonEnemy();
	UPROPERTY(EditDefaultsOnly)
	class UBowMeshComponent* BowMesh;
};
