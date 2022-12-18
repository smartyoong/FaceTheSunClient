// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSocket.h"
#include "FaceTheSunInstance.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UFaceTheSunInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	OnlineSocket sock;
	FText ID;
public:
	OnlineSocket GetSock();
	void InitSock();
	void DeleteSock();
	void SetCharacterName(FText Name);
	FText GetCharacterName();
};
