// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FaceTheSunPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AFaceTheSunPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	bool IsDead = false;
};
