// Copyright Epic Games, Inc. All Rights Reserved.

#include "FaceTheSunGameMode.h"
#include "FaceTheSunCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFaceTheSunGameMode::AFaceTheSunGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
