// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLevelScriptActor.h"
#include "HUDUserWidget.h"

void AMainLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	HUDWidget = Cast<UHUDUserWidget>(CreateWidget<UHUDUserWidget>(GetWorld(), UI_HUD));
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void AMainLevelScriptActor::EndPlay(const EEndPlayReason::Type EndReason)
{
	Super::EndPlay(EndReason);
	if (HUDWidget)
		HUDWidget->RemoveFromParent();
}