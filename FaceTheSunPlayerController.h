// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FaceTheSunPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AFaceTheSunPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void GoToWinMenu();
	void GoToDefeatMenu();
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UDefeatUserWidget>UI_Defeat;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UVictoryUserWidget>UI_Victory;
	UPROPERTY() class UDefeatUserWidget* DefeatView;
	UPROPERTY() class UVictoryUserWidget* VictoryView;
};
