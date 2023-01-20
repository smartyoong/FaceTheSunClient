// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAiContoroller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CommonEnemy.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "FaceTheSunCharacter.h"

ACommonAiContoroller::ACommonAiContoroller()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlakcBoard"));
	checkf(BlackboardComp, TEXT("Blackboard is null"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	checkf(BehaviorTreeComp, TEXT("BehaviorTree is null"));
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI_Sight"));
	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 550.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ACommonAiContoroller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Enemy = Cast<ACommonEnemy>(InPawn);
	if (Enemy)
	{
		// 해당 캐릭터의 AI로 컨트롤을 초기화
		if (!BlackboardComp->InitializeBlackboard(*(Enemy->BehaviorTree->BlackboardAsset)))
			UE_LOG(LogTemp, Log, TEXT("Fail To InitializeBalckbaord"));
		BehaviorTreeComp->StartTree(*(Enemy->BehaviorTree));
	}
}

UBlackboardComponent* ACommonAiContoroller::GetBlackboard()
{
	return BlackboardComp;
}
void ACommonAiContoroller::OnDectectedEnemyBySight(AActor* Actor, FAIStimulus stimulus)
{
	AFaceTheSunCharacter* FC = Cast<AFaceTheSunCharacter>(Actor);
	if (FC && !FC->IsDead)
	{
		// somebody out
		if (Cast<AFaceTheSunCharacter>(GetBlackboard()->GetValueAsObject("TargetToFollow")))
		{
			GetBlackboard()->SetValueAsObject(TEXT("TargetToFollow"), nullptr);
		}
		else //somebody in
		{
			SetFocus(stimulus.WasSuccessfullySensed() ? FC : nullptr);
			if (stimulus.WasSuccessfullySensed())
			{
				bFindEnemy = true;
				if (Enemy)
					Enemy->MultiSetFocus();
				checkf(GetBlackboard(), TEXT("Balckboard is null"));
				GetBlackboard()->SetValueAsObject(TEXT("TargetToFollow"), FC);
				GetBlackboard()->SetValueAsVector(TEXT("MoveToLocation"), FC->GetActorLocation());
			}
			else
				bFindEnemy = false;
		}
	}
}

ETeamAttitude::Type ACommonAiContoroller::GetTeamAttitudeTowards(const AActor& other) const
{
	const APawn* OtherPawn = Cast<APawn>(&other);
	if (OtherPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	}
	auto PlayerTI = Cast<IGenericTeamAgentInterface>(&other);
	class IGenericTeamAgentInterface* AITI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (AITI == nullptr && PlayerTI == nullptr)
		return ETeamAttitude::Neutral;
	FGenericTeamId OtherActorTeamId = NULL;
	if (AITI)
		OtherActorTeamId = AITI->GetGenericTeamId();
	else if (PlayerTI)
		OtherActorTeamId = PlayerTI->GetGenericTeamId();
	FGenericTeamId ThisID = GetGenericTeamId();
	if (ThisID == OtherActorTeamId)
		return ETeamAttitude::Friendly;
	else if(OtherActorTeamId == 1)
		return ETeamAttitude::Hostile;
	else
		return ETeamAttitude::Neutral;
}

void ACommonAiContoroller::BeginPlay()
{
	Super::BeginPlay();
	Enemy = Cast<ACommonEnemy>(GetPawn());
	if (Enemy)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ACommonAiContoroller::OnDectectedEnemyBySight);
		TeamId = FGenericTeamId(Enemy->ID);
	}
}
void ACommonAiContoroller::StopBTTree()
{
	UnPossess();
	Destroy();
}