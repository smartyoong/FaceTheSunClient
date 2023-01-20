// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceTheSunPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DefeatUserWidget.h"
#include "VictoryUserWidget.h"

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
	DefeatView = Cast<UDefeatUserWidget>(CreateWidget<UDefeatUserWidget>(GetWorld(), UI_Defeat));
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	if(DefeatView)
		DefeatView->AddToViewport();
}