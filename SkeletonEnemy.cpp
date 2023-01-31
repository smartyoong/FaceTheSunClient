// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemy.h"
#include "BowMeshComponent.h"

ASkeletonEnemy::ASkeletonEnemy()
{
    BowMesh = CreateDefaultSubobject<UBowMeshComponent>(TEXT("Bow"));
    BowMesh->SetupAttachment(GetMesh(), FName(TEXT("SKT_Bow")));
}