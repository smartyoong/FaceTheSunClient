// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonAiContoroller.h"
#include "BossKarakenAiContoroller.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ABossKarakenAiContoroller : public ACommonAiContoroller
{
	GENERATED_BODY()
public:
	class ABossKraken* Boss;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	bool IsUnderHP = false;
	bool IsSoundStart = false;
};
