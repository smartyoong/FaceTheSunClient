// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayer.h"
#include "CommonAiContoroller.h"
#include "FaceTheSunCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "CommonEnemy.h"

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ThisController = Cast<ACommonAiContoroller>(OwnerComp.GetAIOwner());
	if (ThisController)
	{
		if (ThisController->bFindEnemy)
		{
			auto DetectedCharacter = Cast<AFaceTheSunCharacter>(ThisController->GetBlackboard()->GetValueAsObject("TargetToFollow"));
			if (DetectedCharacter)
			{
				auto AIEnemy = Cast<ACommonEnemy>(OwnerComp.GetAIOwner()->GetPawn());
				if (AIEnemy)
				{
					float CompareRange = AIEnemy->GetDistanceTo(DetectedCharacter);
					if (CompareRange < AIEnemy->GetAttackRange())
					{
						ThisController->GetBlackboard()->SetValueAsBool(TEXT("CanAttack"), true);
					}
					else
					{
						ThisController->GetBlackboard()->SetValueAsBool(TEXT("CanAttack"), false);
						ThisController->GetBlackboard()->SetValueAsVector(TEXT("MoveToLocation"), DetectedCharacter->GetActorLocation());
					}
				}
			}
		}
	}
}