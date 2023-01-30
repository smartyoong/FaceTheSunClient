// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemy.h"
#include "BowMeshComponent.h"

ASkeletonEnemy::ASkeletonEnemy()
{
    BowMesh = CreateDefaultSubobject<UBowMeshComponent>(TEXT("Bow"));
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
    BowMesh->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("SKT_Bow")));
}