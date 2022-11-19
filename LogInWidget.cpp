// Fill out your copyright notice in the Description page of Project Settings.


#include "LogInWidget.h"
#include "SignInWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"

void ULogInWidget::OnLogInButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	ET_ID->GetText();
	ET_Password->GetText();
	this->RemoveFromParent();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("FirstPersonMap"));
	this->Destruct();
}

void ULogInWidget::OnSignInButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	//if(signIn)
		//signIn->AddToViewport();
	this->RemoveFromParent();
	this->Destruct();
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
	//signIn = Cast<USignInWidget>(CreateWidget<USignInWidget>(this, UI_Sign));
}
