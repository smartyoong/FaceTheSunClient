// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FaceTheSunInstance.h"
#include "FaceTheSunGameMode.generated.h"

UCLASS(minimalapi)
class AFaceTheSunGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFaceTheSunGameMode();
	OnlineSocket NetWorkSocket;
};



