// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateLobbyWidget.h"
#include "FaceTheSunInstance.h"
#include "RoomWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

void UCreateLobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (B_Create)
	{
		B_Create->OnClicked.AddDynamic(this, &UCreateLobbyWidget::OnCreateClicked);
	}
	if (B_GoBack)
	{
		B_GoBack->OnClicked.AddDynamic(this, &UCreateLobbyWidget::OnGoBackClicked);
	}
	Instance = Cast<UFaceTheSunInstance>(GetGameInstance());
	RoomWidget = Cast<URoomWidget>(CreateWidget<URoomWidget>(GetWorld(), UI_Room));
}

void UCreateLobbyWidget::OnGoBackClicked() 
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	this->RemoveFromParent();
}

void UCreateLobbyWidget::OnCreateClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	FText RoomName = ET_Title->GetText();
	FText HostName = Instance->GetCharacterName();
	std::string StringRoom = TCHAR_TO_ANSI(*RoomName.ToString());
	std::string StringHost = TCHAR_TO_ANSI(*HostName.ToString());
	RoomInfo info(StringRoom,StringHost);
	PackToBuffer pb(sizeof(PacketID::AskCreateRoom)+sizeof(info));
	pb << PacketID::AskCreateRoom;
	pb.Serialize(info);
	if(Instance->GetSock().GetSock()==INVALID_SOCKET)
		UE_LOG(LogTemp, Log, TEXT("%d"), WSAGetLastError());
	if (Instance->GetSock().Send(&pb) == SOCKET_ERROR)
	{
		UE_LOG(LogTemp, Log, TEXT("SendError"));
		UE_LOG(LogTemp, Log, TEXT("%d"),WSAGetLastError());
	}
	Instance->SetRoomInfo(info);
	RoomWidget->AddToViewport();
}