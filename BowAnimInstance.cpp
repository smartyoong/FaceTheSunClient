// Fill out your copyright notice in the Description page of Project Settings.


#include "BowAnimInstance.h"
#include "SkeletalBowProjectile.h"
#include "SkeletonEnemy.h"
#include "BowMeshComponent.h"

void UBowAnimInstance::AnimNotify_ArrowAttack()
{
    ASkeletonEnemy* Pawn = Cast<ASkeletonEnemy>(TryGetPawnOwner());
	if (::IsValid(Pawn))
    {
        if(ProjectileClass)
        {
            FActorSpawnParameters ActorSpawnParams;
		    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
            ASkeletalBowProjectile* PP = GetWorld()->SpawnActor<ASkeletalBowProjectile>(ProjectileClass, Pawn->BowMesh->GetSocketLocation(TEXT("Arrow")), Pawn->GetActorRotation(), ActorSpawnParams);
        }
    }
}
