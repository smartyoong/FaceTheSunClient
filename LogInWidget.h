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
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_ID; // 아이디
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_Password; // 비밀번호
	UPROPERTY(meta = (BindWidget)) class UButton* B_LogIn; // 로그인 버튼
	UPROPERTY(meta = (BindWidget)) class UButton* B_SignIn; //회원가입 버튼
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // 클릭 효과음
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class USignInWidget>UI_Sign; // 회원가입 블루프린트
	UPROPERTY() class USignInWidget* signIn; // 회원가입  UI 반드시 필수! UPROPERTY선언 안할경우 자동으로 삭제처리
	class UFaceTheSunInstance* Instance; //소켓용

public:
	UFUNCTION(BlueprintCallable) void OnLogInButtonClicked();
	UFUNCTION(BlueprintCallable) void OnSignInButtonClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
