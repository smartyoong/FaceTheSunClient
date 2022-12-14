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
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class ULogInWidget>UI_LogIn; // �α��� �������Ʈ
	UPROPERTY() class ULogInWidget* logIn; // �α��� ��ü �ݵ�� �ʼ�! UPROPERTY���� ���Ұ�� �ڵ����� ����ó��
};
