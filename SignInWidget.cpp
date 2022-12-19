// Fill out your copyright notice in the Description page of Project Settings.


#include "SignInWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "LogInWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunInstance.h"

void USignInWidget::OnIDCheckButtonClicked()
{
	auto text = ET_ID->GetText();
	std::string s = TCHAR_TO_ANSI(*text.ToString());
	PackToBuffer pb(sizeof(s)+sizeof(PacketID::TryID)); // ID확인 전송용
	pb << PacketID::TryID << s;
	FaceTheSunMode->GetSock().Send(&pb);
	int IsOrder = 0;
	FaceTheSunMode->GetSock().Recv(&pb);
	pb >> &IsOrder >> &IsUniqueID;
	if(IsUniqueID == 0)
	{
		FString b(TEXT("해당 아이디는 이미 사용중입니다."));
		FText a = FText::FromString(b);
		T_Ok->SetText(a);
	}
	else
	{
		FString b(TEXT("이 아이디를 사용 하실 수 있습니다!"));
		FText a = FText::FromString(b);
		T_Ok->SetText(a);
	}
}

void USignInWidget::OnSignInButtonClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	bIsSamePassword = ET_Password->GetText().ToString().Equals(ET_PasswordVerify->GetText().ToString());
	if (bIsSamePassword && IsUniqueID==1)
	{
		auto text = ET_ID->GetText();
		auto ptext = ET_Password->GetText();
		std::string s = TCHAR_TO_ANSI(*text.ToString());
		std::string ss = TCHAR_TO_ANSI(*ptext.ToString());
		PackToBuffer pb(sizeof(s) + sizeof(ss) + sizeof(PacketID::TryLogIn)); // 로그인 전송용
		pb << PacketID::TrySignIn << s << ss;
		FaceTheSunMode->GetSock().Send(&pb);
		FaceTheSunMode->GetSock().Recv(&pb); // 어차피 클라이언트 측에서 검사를 다했기때문에 버퍼를 사용할 일은 없음
		int Order = 0;
		int Result = 0;
		pb >> &Order >> &Result;
		if (IsValid(LogInWidget))
		{
			T_Ok->SetText(FText::FromString(TEXT("회원가입 성공")));
			this->RemoveFromParent();
		}
	}
	if(!bIsSamePassword || IsUniqueID==0)
	{
		if (IsUniqueID)
		{
			T_Ok->SetText(FText::FromString(TEXT("비밀번호가 서로 일치하지 않습니다.")));
		}
		else
		{
			T_Ok->SetText(FText::FromString(TEXT("ID를 사용할 수 없습니다.")));
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
	FaceTheSunMode = Cast<UFaceTheSunInstance>(GetGameInstance());
}
