// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KrakenSmashActor.generated.h"

UCLASS()
class FACETHESUN_API AKrakenSmashActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKrakenSmashActor();
	UPROPERTY(EditDefaultsOnly, Category = Smash)
	class UKrakenGoToAirSkill* SmashCollision;
	UPROPERTY(EditDefaultsOnly, Category = Smash)
	class UParticleSystemComponent* SmashEffect;
	UPROPERTY(EditDefaultsOnly, Category = Smash)
	class UAudioComponent* SmashSound;
	void SmashAttack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
