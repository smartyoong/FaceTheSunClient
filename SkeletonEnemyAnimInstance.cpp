// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonEnemyAnimInstance.h"
#include "SkeletonEnemy.h"
#include "FaceTheSunCharacter.h"
#include "CommonAiContoroller.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BowMeshComponent.h"
#include "BowAnimInstance.h"

void USkeletonEnemyAnimInstance::AnimNotify_BowAttack()
{
    auto Pawn = Cast<ASkeletonEnemy>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		ASkeletonEnemy* Enemy = Cast<ASkeletonEnemy>(Pawn);
		if (Enemy)
		{
            auto EAC = Cast<ACommonAiContoroller>(Pawn->GetController());
            if(EAC)
            {
                AFaceTheSunCharacter* FC = Cast<AFaceTheSunCharacter>(EAC->GetBlackboard()->GetValueAsObject("TargetToFollow"));
                if(FC)
                {
                    UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(Enemy->BowMesh->GetAnimInstance());
                    if(BowAnim)
                    {
                        if(Enemy->BowMesh->AttackAnimation)
                            BowAnim->Montage_Play(Enemy->BowMesh->AttackAnimation);
                    }
                }
            }
		}
	}
}