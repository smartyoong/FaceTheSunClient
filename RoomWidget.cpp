// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomWidget.h"
#include "FaceTheSunInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void URoomWidget::OnStartClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
}

void URoomWidget::OnGoBackClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	VB_User->ClearChildren();
	this->RemoveFromParent();
}

void URoomWidget::OnSendChatClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
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
}