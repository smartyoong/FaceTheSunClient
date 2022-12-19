// Fill out your copyright notice in the Description page of Project Settings.


#include "LogInWidget.h"
#include "SignInWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunGameMode.h"
#include "FaceTheSunInstance.h"

void ULogInWidget::OnLogInButtonClicked() // 로그인 시도
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	auto text = ET_ID->GetText();
	auto ptext = ET_Password->GetText();
	std::string s = TCHAR_TO_ANSI(*text.ToString());
	std::string ss = TCHAR_TO_ANSI(*ptext.ToString());
	PackToBuffer pb(sizeof(s)+sizeof(ss)+sizeof(PacketID::TryLogIn)); // 로그인 전송용
	pb << PacketID::TryLogIn << s << ss;
	Instance->GetSock().Send(&pb);
	int IsLoginOrder = 0;
	int LoginResult = 0;
	Instance->GetSock().Recv(&pb);
	pb >> &IsLoginOrder >> &LoginResult;
	UE_LOG(LogTemp, Log, TEXT("%d %d"), IsLoginOrder, LoginResult);
	if (IsLoginOrder == 1)
	{
		if (LoginResult)
		{
			Instance->SetCharacterName(text);
			this->RemoveFromParent();
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainLobby"));
			this->Destruct();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("LogInFail"));
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("LogIn Try Fail"));
	}
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
	Instance = Cast<UFaceTheSunInstance>(GetGameInstance());
	Instance ->GetSock().Init();
	Instance ->GetSock().Connect();
}

void ULogInWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
