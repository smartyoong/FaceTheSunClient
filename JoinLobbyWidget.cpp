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
	int RecvSize;
	pbb >> &IsRecvLooby >> &RecvSize;
	UE_LOG(LogTemp, Log, TEXT("%d"), RecvSize);
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
			UE_LOG(LogTemp, Log, TEXT("%s"), *FString(ri.RoomName.c_str()));
			RoomList.push_back(ri);
			Lobby = NewObject<ULoobyItemUI>(LobbyList);
			/*이부분 개선 필요 지금 현재 UI가 적용이 안됨*/
			LobbyList->AddChild(Lobby);
			//Lobby->SetRoomName(ri);
		}
	}
}

void UJoinLobbyWidget::OnRefreshClicked()
{
	LobbyList->ClearChildren();
	RoomList.clear();
	RecvLobby();
}
