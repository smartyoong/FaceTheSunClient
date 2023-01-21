// Fill out your copyright notice in the Description page of Project Settings.


#include "HPItem.h"
#include "TP_PickUpComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "FaceTheSunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AHPItem::AHPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PickComp = CreateDefaultSubobject<UTP_PickUpComponent>(TEXT("CollisionPickComp"));
	RootComponent = PickComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateComp"));
}

// Called when the game starts or when spawned
void AHPItem::BeginPlay()
{
	Super::BeginPlay();
	PickComp->OnPickUp.AddDynamic(this,&AHPItem::HealUp);
}


void AHPItem::HealUp(AFaceTheSunCharacter* ch)
{
	ch->ServerHeal();
	ch->HP += HealHP;
	if (ch->HP > 100)
		ch->HP = 100;
	Destroy();
}