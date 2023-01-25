// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "FaceTheSunGameState.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunInstance.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerState.h"
#include "FaceTheSunPlayerController.h"
#include "FaceTheSunPlayerState.h"

void AMainGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	if (MainGameState->bBossKilled)
	{
		TArray<AActor*>AllActs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActs);
		for (int32 i = 0; i < AllActs.Num(); ++i)
		{
			AFaceTheSunCharacter* AF = Cast<AFaceTheSunCharacter>(AllActs[i]);
			if (AF)
			{
				AF->CallWinFunc();
			}
		}
	}
	else
	{
		TArray<AActor*>AllActs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActs);
		for (int32 i = 0; i < AllActs.Num(); ++i)
		{
			AFaceTheSunCharacter* AF = Cast<AFaceTheSunCharacter>(AllActs[i]);
			if (AF)
			{
				AF->CallDefeatFunc();
			}
		}
	}
}

bool AMainGameMode::ReadyToEndMatch_Implementation()
{
	bool IsLose = false;
	bool IsWin = false;
	auto PlayerStateArray = MainGameState->PlayerArray;
	if (PlayerStateArray.Num()) // 1명이라도 있는가?
	{
		// 다 죽어야만 리턴 만약 1명이라도 안죽었다면 false로 변함
		IsLose = true;
		for (int32 i = 0; i < PlayerStateArray.Num(); ++i)
		{
			auto PS = Cast<AFaceTheSunPlayerState>(PlayerStateArray[i]);
			if (PS)
			{
				if (!PS->IsDead)
					IsLose = false;
			}
		}
	}
	if (MainGameState->bBossKilled)
	{
		IsWin =  true;
	}
	return IsLose || IsWin;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	MainGameState = GetGameState<AFaceTheSunGameState>();
	bReplicates = true;
}

void AMainGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();
}

void AMainGameMode::AbortMatch()
{
	Super::AbortMatch();
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLobby"), true);
}