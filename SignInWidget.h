// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API USignInWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget)) class UButton* B_SignIn; // ID�ߺ� üũ ��ư
	UPROPERTY(meta = (BindWidget)) class UButton* B_LogIn; // ȸ�� ���� ��ư
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_ID; // ID
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_Password; // ���
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_PasswordVerify; // ���Ȯ��
	UPROPERTY(meta = (BindWidget)) class UTextBlock* T_Ok; // ��� Ȯ�ο�
	//UPROPERTY(EditDefaultsOnly) TSubclassOf<class ULogInWidget> UI_LogIn; // ȸ������ �Ϸ��� �α��� �������� ���ư��� ���� ��������Ʈ
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // Ŭ�� �Ҹ�
	//UPROPERTY() class ULogInWidget* LogIn; //�α��� ���� �ݵ�� �ʼ�! UPROPERTY���� ���Ұ�� �ڵ����� ����ó��
public:
	UFUNCTION(BlueprintCallable) void OnIDCheckButtonClicked();
	UFUNCTION(BlueprintCallable) void OnSignInButtonClicked();
	virtual void NativeOnInitialized() override;
public:
	UPROPERTY(BlueprintReadWrite)bool bIsUniqueID = false;
	UPROPERTY(BlueprintReadWrite)bool bIsSamePassword = false;
};