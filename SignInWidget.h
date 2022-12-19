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
	UPROPERTY(meta = (BindWidget)) class UButton* B_SignIn; // ID중복 체크 버튼
	UPROPERTY(meta = (BindWidget)) class UButton* B_LogIn; // 회원 가입 버튼
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_ID; // ID
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_Password; // 비번
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_PasswordVerify; // 비번확인
	UPROPERTY(meta = (BindWidget)) class UTextBlock* T_Ok; // 결과 확인용
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // 클릭 소리
	UPROPERTY(EditDefaultsOnly, Category = LogInUI) TSubclassOf<class UUserWidget> LogInWidget; //로그인 위젯 반드시 필수! UPROPERTY선언 안할경우 자동으로 삭제처리
	UPROPERTY() class ULogInWidget* LogWidget; // 로그인 위젯
	class UFaceTheSunInstance* FaceTheSunMode; // 소켓용
public:
	UFUNCTION(BlueprintCallable) void OnIDCheckButtonClicked();
	UFUNCTION(BlueprintCallable) void OnSignInButtonClicked();
	virtual void NativeOnInitialized() override;
public:
	UPROPERTY(BlueprintReadWrite)int IsUniqueID = false;
	UPROPERTY(BlueprintReadWrite)bool bIsSamePassword = false;
};
