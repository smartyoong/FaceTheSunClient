// Fill out your copyright notice in the Description page of Project Settings.


#include "DefeatUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunInstance.h"
#include "Components/Button.h"

void UDefeatUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (B_Next)
	{
		B_Next->OnClicked.AddDynamic(this, &UDefeatUserWidget::OnNextClicked);
	}
	Instance = Cast<UFaceTheSunInstance>(GetGameInstance());
}

void UDefeatUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UGameplayStatics::PlaySound2D(GetWorld(), BGMSound,0.3f);
}

void UDefeatUserWidget::OnNextClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	auto CName = Instance->GetCharacterName();
	std::string CharacterName = TCHAR_TO_ANSI(*CName.ToString());
	bool Win = true;
	PackToBuffer pb(sizeof(PacketID::SendGameResult) + sizeof(CharacterName) + sizeof(Win));
	Instance->GetSock().Send(&pb);
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLobby"), true);
	this->RemoveFromParent();
}