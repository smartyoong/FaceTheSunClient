// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LogInLevelSetting.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ALogInLevelSetting : public ALevelScriptActor
{
	GENERATED_BODY()

public:

	ALogInLevelSetting();
	virtual void BeginPlay() override;

public:
	USoundBase* LogAndSignInMusic; // bgm
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class ULogInWidget>UI_LogIn; // 로그인 블루프린트
	UPROPERTY() class ULogInWidget* logIn; // 로그인 객체 반드시 필수! UPROPERTY선언 안할경우 자동으로 삭제처리
};
