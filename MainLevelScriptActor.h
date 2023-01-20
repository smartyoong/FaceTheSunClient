// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AMainLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UHUDUserWidget>UI_HUD; // 방 블루프린트
	UPROPERTY() class UHUDUserWidget* HUDWidget;
	virtual void EndPlay(const EEndPlayReason::Type EndReason) override;
};
