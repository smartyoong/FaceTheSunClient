// Fill out your copyright notice in the Description page of Project Settings.


#include "LoobyItemUI.h"
#include "FaceTheSunInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void ULoobyItemUI::SetRoomName(struct RoomInfo info)
{
	TB_Room->SetText(FText::FromString(FString(info.RoomName.c_str())));
}

void ULoobyItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULoobyItemUI::NativeConstruct()
{
	Super::NativeConstruct();
}
void ULoobyItemUI::OnJoinClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
}
