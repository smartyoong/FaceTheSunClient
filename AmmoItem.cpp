// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoItem.h"
#include "TP_PickUpComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "FaceTheSunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "TP_WeaponComponent.h"

// Sets default values
AAmmoItem::AAmmoItem()
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
void AAmmoItem::BeginPlay()
{
	Super::BeginPlay();
	PickComp->OnPickUp.AddDynamic(this, &AAmmoItem::AmmoUp);
}

void AAmmoItem::AmmoUp(AFaceTheSunCharacter* ch)
{
	ch->ServerAmmo();
	Destroy();
}

