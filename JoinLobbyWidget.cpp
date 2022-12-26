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
#include "RoomWidget.h"

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
	RoomWidget = Cast<URoomWidget>(CreateWidget<URoomWidget>(GetWorld(), UI_Room));
	Instance = Cast<UFaceTheSunInstance>(GetGameInstance());
}

void UJoinLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
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
	PackToBuffer pb(sizeof(PacketID::AskToRoom) + sizeof(Data->GetRoomInfo().RoomName) + sizeof(TCHAR_TO_ANSI(*Instance->GetCharacterName().ToString()+sizeof(int))));
	std::string CharacterNameString = TCHAR_TO_ANSI(*Instance->GetCharacterName().ToString());
	pb << PacketID::AskToRoom << Data->GetRoomInfo().HostName << CharacterNameString << (int)LobbyList->GetIndexForItem(LobbyList->GetSelectedItem());;
	Instance->GetSock().Send(&pb);
	PackToBuffer pbb(1024);
	Instance->GetSock().Recv(&pbb);
	int IsRecvRoom = 0;
	int CurrentPlayerCount = 0;
	bool CanJoinRoom = false;
	pbb >> &IsRecvRoom >> &CanJoinRoom >> &CurrentPlayerCount;
	if (IsRecvRoom == PacketID::SendRoomInfo)
	{
		if (CanJoinRoom)
		{
			RoomInfo info;
			pbb.DeSerialize(&info);
			for (int i = 0; i < CurrentPlayerCount; ++i)
			{
				std::string s;
				pbb >> &s;
				Instance->MultiPlayerNames.push_back(FText::FromString(FString(s.c_str())));
			}
			Instance->SetRoomInfo(info);
			RoomWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("방 입장이 불가능합니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("방 입장중 통신 오류 발생"));
	}
}
