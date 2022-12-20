// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinLobbyWidget.h"
#include "FaceTheSunInstance.h"
#include "LoobyItemUI.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"

void UJoinLobbyWidget::OnGoBackClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	LobbyList->ClearChildren();
	RoomList.clear();
	this->RemoveFromParent();
}
void UJoinLobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (B_GoBack)
		B_GoBack->OnClicked.AddDynamic(this, &UJoinLobbyWidget::OnGoBackClicked);
	if (B_Lobby)
		B_Lobby->OnClicked.AddDynamic(this, &UJoinLobbyWidget::OnRefreshClicked);
}

void UJoinLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Instance = Cast<UFaceTheSunInstance>(GetGameInstance());
	RecvLobby();
}

void UJoinLobbyWidget::RecvLobby()
{
	PackToBuffer pb(sizeof(PacketID::AskLobby)+1);
	pb << PacketID::AskLobby;
	PackToBuffer pbb(8192);
	Instance->GetSock().Send(&pb);
	Instance->GetSock().Recv(&pbb);
	int IsRecvLooby;
	pbb >> &IsRecvLooby;
	if (IsRecvLooby == PacketID::SendLobby)
	{
		pbb.DeSerialize(&RoomList);
	}
	for (int i = 0; i < RoomList.size(); ++i)
	{
		ULoobyItemUI* LoobyItem = NewObject<ULoobyItemUI>(LobbyList);
		LoobyItem->SetRoomName(RoomList[i]);
		LobbyList->AddChild(LoobyItem);
	}
}

void UJoinLobbyWidget::OnRefreshClicked()
{
	LobbyList->ClearChildren();
	RoomList.clear();
	PackToBuffer pb(sizeof(PacketID::AskLobby)+1);
	pb << PacketID::AskLobby;
	PackToBuffer pbb(8192);
	Instance->GetSock().Send(&pb);
	Instance->GetSock().Recv(&pbb);
	int IsRecvLooby;
	pbb >> &IsRecvLooby;
	if (IsRecvLooby == PacketID::SendLobby)
	{
		pbb.DeSerialize(&RoomList);
	}
	for (int i = 0; i < RoomList.size(); ++i)
	{
		ULoobyItemUI* LoobyItem = NewObject<ULoobyItemUI>(LobbyList);
		LoobyItem->SetRoomName(RoomList[i]);
		LobbyList->AddChild(LoobyItem);
	}
}
