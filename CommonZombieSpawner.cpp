// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonZombieSpawner.h"
#include "CommonEnemy.h"

// Sets default values
ACommonZombieSpawner::ACommonZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACommonZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
}
void ACommonZombieSpawner::StartSpawn()
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn the projectile at the muzzle
	GetWorld()->SpawnActor<ACommonEnemy>(EnemyClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &ACommonZombieSpawner::StartSpawn, DelayTime, false);
}
void ACommonZombieSpawner::StopSpawn()
{
	Destroy();
}

