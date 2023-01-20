// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FaceTheSunGameState.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AFaceTheSunGameState : public AGameState
{
	GENERATED_BODY()
private:
		TArray<class AFaceTheSunCharacter*> CharacterList;
public:
	void AddPlayerToList(class AFaceTheSunCharacter* ch);
	TArray<class AFaceTheSunCharacter*> GetPlayerList();
	bool bBossKilled = false;
};
