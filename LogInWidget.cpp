// Fill out your copyright notice in the Description page of Project Settings.


#include "LogInWidget.h"
#include "SignInWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"

void ULogInWidget::OnLogInButtonClicked() // 로그인 시도
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	auto text = ET_ID->GetText();
	auto ptext = ET_Password->GetText();
	this->RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FirstPersonMap"));
	this->Destruct();
}

void ULogInWidget::OnSignInButtonClicked() //회원가입 버튼
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	if (IsValid(UI_Sign))
	{
		signIn = Cast<USignInWidget>(CreateWidget(GetWorld(), UI_Sign));
		if (signIn != nullptr)
		{
			signIn->AddToViewport();
		}
	}
}

void ULogInWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (B_LogIn)
	{
		B_LogIn->OnClicked.AddDynamic(this, &ULogInWidget::OnLogInButtonClicked);
	}
	if (B_SignIn)
	{
		B_SignIn->OnClicked.AddDynamic(this, &ULogInWidget::OnSignInButtonClicked);
	}
}
