// Fill out your copyright notice in the Description page of Project Settings.


#include "LoobyItemUI.h"
#include "FaceTheSunInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RoomListItemData.h"
#include "Kismet/GameplayStatics.h"


void ULoobyItemUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULoobyItemUI::NativeConstruct()
{
	Super::NativeConstruct();
}
void ULoobyItemUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	URoomListItemData* Data = Cast<URoomListItemData>(ListItemObject);
	if (Data)
	{
		TB_Room->Font.Size = 40;
		TB_Room->SetText(FText::FromString(FString(Data->GetRoomInfo().RoomName.c_str())));
	}
}
void ULoobyItemUI::NativeOnItemSelectionChanged(bool bIsSelected)
{
	TB_Room->SetColorAndOpacity(FSlateColor(FLinearColor(3.0f / 255.0f, 252.0f / 255.0f, 11.0f / 255.0f, 1.0f)));
}