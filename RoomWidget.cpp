// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "ThreadTaskRoom.h"

void URoomWidget::OnStartClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
}

void URoomWidget::OnGoBackClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	VB_User->ClearChildren();
	SB_Chat->ClearChildren();
	delete tp; // ������ ���ŷ �߻�
	Instance->MultiPlayerNames.clear();
	Instance->OrderQue.clear();
	//VB_User->RemoveChild(����);
	this->RemoveFromParent();
}

void URoomWidget::OnSendChatClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	FText Chat = ET_Chat->GetText();
	std::string ChatString = TCHAR_TO_UTF8(*Chat.ToString());
	PackToBuffer pb(sizeof(PacketID::SendChat) + sizeof(ChatString)+sizeof(Instance->GetRoomInfo().HostName)+sizeof(TCHAR_TO_UTF8 (*Instance->GetCharacterName().ToString())));
	std::string Character = TCHAR_TO_UTF8(*Instance->GetCharacterName().ToString());
	pb << PacketID::SendChat << Instance->GetRoomInfo().HostName << Character <<ChatString;
	Instance->GetSock().Send(&pb);
}

void URoomWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (B_Start)
	{
		B_Start->OnClicked.AddDynamic(this, &URoomWidget::OnStartClicked);
	}
	if (B_Send)
	{
		B_Send->OnClicked.AddDynamic(this, &URoomWidget::OnSendChatClicked);
	}
	if (B_GoBack)
	{
		B_GoBack->OnClicked.AddDynamic(this, &URoomWidget::OnGoBackClicked);
	}
}

void URoomWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Instance = Cast<UFaceTheSunInstance>(GetGameInstance());
	if (Instance == nullptr)
		UE_LOG(LogTemp, Log, TEXT("Instance"));
	HostName = Instance->GetCharacterName();
	TB_RoomName->SetText(FText::FromString(FString(Instance->GetRoomInfo().RoomName.c_str())));
	UTextBlock* NewTB = NewObject<UTextBlock>(VB_User);
	NewTB->Font.Size = 200;
	NewTB->SetText(HostName);
	VB_User->AddChild(NewTB);
	for (int i = 1; i < Instance->MultiPlayerNames.size(); ++i)
	{
		UTextBlock* NNewTB = NewObject<UTextBlock>(VB_User);
		NNewTB->Font.Size = 200;
		NNewTB->SetText(Instance->MultiPlayerNames[i]);
		VB_User->AddChild(NNewTB);
	}
	tp = new ThreadTaskRoom(this);
}

void URoomWidget::AddNewUserName(PackToBuffer& pb)
{
	std::string Name;
	pb >> &Name;
	UTextBlock* NewTB = NewObject<UTextBlock>(VB_User);
	NewTB->Font.Size = 200;
	NewTB->SetText(FText::FromString(FString(Name.c_str())));
	VB_User->AddChild(NewTB);
}

void URoomWidget::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
	Super::NativeTick(Geometry, DeltaSeconds);
	if (Instance->OrderQue.unsafe_size() > 0)
	{
		PackToBuffer pb(1024);
		Instance->OrderQue.try_pop(pb);
		int KindOfOrder;
		pb >> &KindOfOrder;
		switch (KindOfOrder)
		{
		case PacketID::SomeBodyJoin:
			AddNewUserName(pb);
			break;
		case PacketID::RecvChat:
			AddChat(pb);
			break;
		default:
			break;
		}
	}
}

void URoomWidget::AddChat(PackToBuffer& pb)
{
	std::string Chat;
	pb >> &Chat;
	UTextBlock* NewTB = NewObject<UTextBlock>(SB_Chat);
	NewTB->Font.Size = 30;
	NewTB->SetText(FText::FromString(FString(Chat.c_str())));
	SB_Chat->ScrollToEnd();
	SB_Chat->AddChild(NewTB);
}