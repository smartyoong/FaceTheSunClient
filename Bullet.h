// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FaceTheSunProjectile.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ABullet : public AFaceTheSunProjectile
{
	GENERATED_BODY()
public:
	ABullet();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
