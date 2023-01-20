// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
	virtual void HandleLeavingMap() override;
	virtual void HandleMatchHasEnded() override;
	virtual void BeginPlay() override;
	virtual bool ReadyToEndMatch_Implementation() override;
	virtual void AbortMatch() override;
	UPROPERTY()
	class AFaceTheSunGameState* MainGameState;
};
