// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceTheSunGameState.h"
#include "FaceTheSunCharacter.h"

void AFaceTheSunGameState::AddPlayerToList(class AFaceTheSunCharacter* ch)
{
	CharacterList.Add(ch);
}
TArray<class AFaceTheSunCharacter*> AFaceTheSunGameState::GetPlayerList()
{
	return CharacterList;
}