// Fill out your copyright notice in the Description page of Project Settings.


#include "KrakenSmashActor.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FaceTheSunCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "KrakenGoToAirSkill.h"

// Sets default values
AKrakenSmashActor::AKrakenSmashActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SmashCollision = CreateDefaultSubobject<UKrakenGoToAirSkill>(TEXT("Collision"));
	RootComponent = SmashCollision;
	SmashSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SmashSound"));
	SmashSound->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepWorldTransform);
    SmashEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmashEffect"));
	SmashEffect->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void AKrakenSmashActor::BeginPlay()
{
	Super::BeginPlay();
}

void AKrakenSmashActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(SmashCollision->IsFinish)
	{
    	Destroy();
	}
}
