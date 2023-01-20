// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "CommonAiContoroller.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ACommonAiContoroller : public AAIController
{
	GENERATED_BODY()
public:
	ACommonAiContoroller();
	virtual void OnPossess(APawn* InPawn) override;
	UBlackboardComponent* GetBlackboard();
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComp;
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	virtual void BeginPlay() override;
	void StopBTTree();
	bool bFindEnemy = false;
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
	class UBlackboardComponent* BlackboardComp;
	UPROPERTY(BlueprintReadWrite, Category = "Behavior")
	class UBehaviorTreeComponent* BehaviorTreeComp;
	class UAISenseConfig_Sight* SightConfig;
	class ACommonEnemy* Enemy;
	UFUNCTION()
	void OnDectectedEnemyBySight(AActor* Actor, FAIStimulus stimulus);
	FGenericTeamId TeamId;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& other) const override;
};
