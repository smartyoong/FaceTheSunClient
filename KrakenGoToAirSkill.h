// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "KrakenGoToAirSkill.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UKrakenGoToAirSkill : public USphereComponent
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SmashBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPlay() override;
	bool IsFinish = false;

};
