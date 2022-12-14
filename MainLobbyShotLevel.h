// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainLobbyShotLevel.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API AMainLobbyShotLevel : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, Category = "BGM") class USoundBase* audioComponent; //bgm
	
};
