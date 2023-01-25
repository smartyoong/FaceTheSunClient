// Fill out your copyright notice in the Description page of Project Settings.


#include "BossKarakenAiContoroller.h"
#include "BossKraken.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABossKarakenAiContoroller::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    Boss = Cast<ABossKraken>(InPawn);
}

void ABossKarakenAiContoroller::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    //나중에 비율 계산할것
    float CurrHPRate =(Boss->HP/Boss->StartHP);
    if( CurrHPRate < 0.3f)
    {
        IsUnderHP = true;
        if(!IsSoundStart)
        {
            Boss->MultiStopBGMSound();
            Boss->MultiHPUnder30();
            IsSoundStart = true;
        }
        GetBlackboard()->SetValueAsBool(TEXT("IsLowHP"),IsUnderHP);
    }
    if((Boss->HP/Boss->StartHP*100) > 30)
    {
        IsUnderHP = false;
        GetBlackboard()->SetValueAsBool(TEXT("IsLowHP"),IsUnderHP);
    }
}
