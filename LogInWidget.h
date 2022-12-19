// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LogInWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ULogInWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_ID; // ���̵�
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_Password; // ��й�ȣ
	UPROPERTY(meta = (BindWidget)) class UButton* B_LogIn; // �α��� ��ư
	UPROPERTY(meta = (BindWidget)) class UButton* B_SignIn; //ȸ������ ��ư
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // Ŭ�� ȿ����
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class USignInWidget>UI_Sign; // ȸ������ �������Ʈ
	UPROPERTY() class USignInWidget* signIn; // ȸ������  UI �ݵ�� �ʼ�! UPROPERTY���� ���Ұ�� �ڵ����� ����ó��
	class UFaceTheSunInstance* Instance; //���Ͽ�

public:
	UFUNCTION(BlueprintCallable) void OnLogInButtonClicked();
	UFUNCTION(BlueprintCallable) void OnSignInButtonClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
