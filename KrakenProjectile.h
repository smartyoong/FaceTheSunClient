// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FaceTheSunProjectile.h"
#include "KrakenProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AKrakenProjectile : public AFaceTheSunProjectile
{
	GENERATED_BODY()
public:
	AKrakenProjectile();
	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* HomingEffect;
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* HomingSound;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void BeginPlay() override;
};
