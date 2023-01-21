// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskCommonAttack.h"
#include "CommonEnemy.h"
#include "CommonAiContoroller.h"

EBTNodeResult::Type UBTTaskCommonAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ThisController = Cast<ACommonAiContoroller>(OwnerComp.GetAIOwner());
	ACommonEnemy* Enemy = Cast<ACommonEnemy>(ThisController->GetPawn());
	if (ThisController == nullptr || Enemy == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Task: AIcontrl or AI Character is null"));
		return EBTNodeResult::Failed;
	}
	Enemy->MultiAttack();
	return EBTNodeResult::Succeeded;
}