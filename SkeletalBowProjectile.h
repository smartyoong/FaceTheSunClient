// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FaceTheSunProjectile.h"
#include "SkeletalBowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ASkeletalBowProjectile : public AFaceTheSunProjectile
{
	GENERATED_BODY()
	
public:
	ASkeletalBowProjectile();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void BeginPlay() override;
};
