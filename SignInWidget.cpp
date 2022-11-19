// Fill out your copyright notice in the Description page of Project Settings.


#include "SignInWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "LogInWidget.h"
#include "Kismet/GameplayStatics.h"

void USignInWidget::OnIDCheckButtonClicked()
{
	if(!bIsUniqueID)
	{
		FString b("You can't use this ID");
		FText a = FText::FromString(b);;
		T_Ok->SetText(a);
		bIsUniqueID = true; //임시로 테스트용으로 해둔것
	}
}

void USignInWidget::OnSignInButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	bIsSamePassword = ET_Password->GetText().ToString().Equals(ET_PasswordVerify->GetText().ToString());
	if (bIsSamePassword && bIsUniqueID)
		T_Ok->SetText(FText::FromString("Are you sure to sign in? Press Ok one more time"));
	if(!bIsSamePassword || !bIsUniqueID)
	{
		if (bIsUniqueID)
		{
			T_Ok->SetText(FText::FromString("Password Incorrect."));
		}
		else
		{
			T_Ok->SetText(FText::FromString("You can't use this ID."));
		}
	}
}

void USignInWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//LogIn = Cast<ULogInWidget>(CreateWidget<ULogInWidget>(this, UI_LogIn));
	if (B_SignIn)
	{
		B_SignIn->OnClicked.AddDynamic(this, &USignInWidget::OnSignInButtonClicked);
	}
	if (B_LogIn)
	{
		B_LogIn->OnClicked.AddDynamic(this, &USignInWidget::OnIDCheckButtonClicked);
	}
}
