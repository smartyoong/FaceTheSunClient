// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "CreateLobbyWidget.h"
#include "JoinLobbyWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "FaceTheSunInstance.h"

void UMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (B_Create)
	{
		B_Create->OnClicked.AddDynamic(this, &UMainWidget::OnCreateLobbyClicked);
	}
	if (B_Join)
	{
		B_Join->OnClicked.AddDynamic(this, &UMainWidget::OnJoinLobbyClicked);
	}
	if (B_Quit)
	{
		B_Quit->OnClicked.AddDynamic(this, &UMainWidget::OnQuitClicked);
	}
	/*
	if (B_Shop)
	{
		B_Shop->OnClicked.AddDynamic(this, &UMainWidget::OnShopClicked);
	}
	*/
	FaceTheSunMode = Cast<UFaceTheSunInstance>(GetGameInstance());
	JoinLobby = Cast<UJoinLobbyWidget>(CreateWidget<UJoinLobbyWidget>(GetWorld(), UI_Join));
	CreateLobby = Cast<UCreateLobbyWidget>(CreateWidget<UCreateLobbyWidget>(GetWorld(), UI_Create));
}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainWidget::OnJoinLobbyClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	if (JoinLobby)
	{
		JoinLobby->AddToViewport();
	}
}

void UMainWidget::OnCreateLobbyClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	if (CreateLobby)
	{
		CreateLobby->AddToViewport();
	}
}

void UMainWidget::OnShopClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
}

void UMainWidget::OnQuitClicked()
{
	FaceTheSunMode->GetSock().Close();
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}