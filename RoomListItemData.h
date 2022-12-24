// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OnlineSocket.h"
#include "RoomListItemData.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API URoomListItemData : public UObject
{
	GENERATED_BODY()
	
private:
	RoomInfo info;
public:
	void SetRoomInfo(struct RoomInfo in) { info = in; };
	RoomInfo GetRoomInfo() { return info; };
};
