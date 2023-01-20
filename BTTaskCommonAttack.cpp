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
	FTimerHandle WaitHandle;
	float WaitTime = 5.f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{GetWorld()->GetTimerManager().ClearTimer(WaitHandle); }), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
	return EBTNodeResult::Succeeded;
}