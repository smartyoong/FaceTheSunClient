// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonZombieSpawner.generated.h"

UCLASS()
class FACETHESUN_API ACommonZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommonZombieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void StartSpawn();
	void StopSpawn();
	FTimerHandle DelayHandle;
	UPROPERTY(EditDefaultsOnly)
	float DelayTime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = Enemy)
	TSubclassOf<class ACommonEnemy> EnemyClass;

};
