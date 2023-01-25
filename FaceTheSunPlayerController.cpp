// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceTheSunPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DefeatUserWidget.h"
#include "VictoryUserWidget.h"
#include "BossKrakenHP.h"

void AFaceTheSunPlayerController::GoToWinMenu()
{
	VictoryView = Cast<UVictoryUserWidget>(CreateWidget<UVictoryUserWidget>(GetWorld(), UI_Victory));
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	if(VictoryView)
	{
		VictoryView->AddToViewport();
	}
}
void AFaceTheSunPlayerController::GoToDefeatMenu()
{
	TArray<AActor*>actarray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),actarray);
	for(auto i : actarray)
	{
		ABossKraken* boss = Cast<ABossKraken>(i);
		if(boss)
			boss->MultiStopBGMSound();
	}
	DefeatView = Cast<UDefeatUserWidget>(CreateWidget<UDefeatUserWidget>(GetWorld(), UI_Defeat));
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	if(BossHPView)
		BossHPView->RemoveFromParent();
	if(DefeatView)
		DefeatView->AddToViewport();
}

void AFaceTheSunPlayerController::ShowBossHP()
{
	BossHPView = Cast<UBossKrakenHP>(CreateWidget<UBossKrakenHP>(GetWorld(), UI_BossHP));
	if(BossHPView)
		BossHPView->AddToViewport();
}