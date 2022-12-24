// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinLobbyWidget.h"
#include "FaceTheSunInstance.h"
#include "LoobyItemUI.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RoomListItemData.h"

void UJoinLobbyWidget::OnGoBackClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	LobbyList->ClearListItems();
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
	if (B_Join)
		B_Join->OnClicked.AddDynamic(this, &UJoinLobbyWidget::OnJoinClicked);
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
	int RecvSize;
	pbb >> &IsRecvLooby >> &RecvSize;
	if (IsRecvLooby == PacketID::SendLobby)
	{
		for (int i = 0; i < RecvSize; ++i)
		{
			int RoomNameSize;
			int HostNameSize;
			pbb >> &RoomNameSize >> &HostNameSize;
			std::string roomname;
			std::string hostname;
			roomname.reserve(RoomNameSize);
			hostname.reserve(HostNameSize);
			int currpl;
			bool canjoin;
			pbb.DeSerialize(&roomname);
			pbb.DeSerialize(&hostname);
			pbb.DeSerialize(&currpl);
			pbb.DeSerialize(&canjoin);
			RoomInfo ri(roomname, hostname);
			ri.CurrentPlayer = currpl;
			ri.CanJoin = canjoin;
			RoomList.push_back(ri);
			URoomListItemData* Data = NewObject<URoomListItemData>();
			Data->SetRoomInfo(ri);
			LobbyList->AddItem(Data);

		}
	}
}

void UJoinLobbyWidget::OnRefreshClicked()
{
	LobbyList->ClearListItems();
	RoomList.clear();
	RecvLobby();
}

void UJoinLobbyWidget::OnJoinClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	auto Data =Cast<URoomListItemData>(LobbyList->GetSelectedItem());
}
