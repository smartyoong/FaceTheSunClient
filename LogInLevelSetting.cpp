// Fill out your copyright notice in the Description page of Project Settings.


#include "LogInLevelSetting.h"
#include "Kismet/GameplayStatics.h"
#include "LogInWidget.h"
#include "Blueprint/UserWidget.h"

ALogInLevelSetting::ALogInLevelSetting()
{
	ConstructorHelpers::FObjectFinder<USoundBase>LogSignSoundTemp(TEXT("/Script/Engine.SoundWave'/Game/LogInMain/Sound/LogInSignIn.LogInSignIn'"));
	if (LogSignSoundTemp.Succeeded())
		LogAndSignInMusic = LogSignSoundTemp.Object;
	//logIn = Cast<ULogInWidget>(CreateWidget<ULogInWidget>(GetWorld(), UI_LogIn));
}
void ALogInLevelSetting::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(GetWorld(), LogAndSignInMusic);
	/*
	if (logIn != nullptr)
	{
		logIn->AddToViewport();
	}
	*/
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->bShowMouseCursor = true;
}
