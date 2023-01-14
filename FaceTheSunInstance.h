// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSocket.h"
#include <vector>
#include <concurrent_queue.h>
#include "FaceTheSunInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UFaceTheSunInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	OnlineSocket NetWorkSocket;
	FText ID;
	RoomInfo info;
public:
	void InitSock();
	void DeleteSock();
	void SetCharacterName(FText Name);
	FText GetCharacterName();
	void SetRoomInfo(RoomInfo room) { info = room; }
	RoomInfo GetRoomInfo() { return info; }
	OnlineSocket& GetSock() { return NetWorkSocket; }
	std::vector<FText> MultiPlayerNames;
	Concurrency::concurrent_queue<PackToBuffer> OrderQue;
	bool IsHost = false;
};
