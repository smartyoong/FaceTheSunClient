// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainLobbyShotLevel.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AMainLobbyShotLevel : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, Category = "BGM") class USoundBase* audioComponent; //bgm
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UMainWidget>UI_Main; // 메인화면 블루프린트
	UPROPERTY() class UMainWidget* Main; // UPROPERTY선언 안할경우 자동으로 삭제처리
};
