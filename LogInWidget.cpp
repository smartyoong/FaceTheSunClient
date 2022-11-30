// Fill out your copyright notice in the Description page of Project Settings.


#include "LogInWidget.h"
#include "SignInWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "NetWorking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"

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
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));
	FString address = TEXT("124.54.76.95");
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);
	int32 port = 18891;
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(port);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));
	bool isConnected = Socket->Connect(*addr);
	if (isConnected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Success")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail To Connect")));
	}
}
