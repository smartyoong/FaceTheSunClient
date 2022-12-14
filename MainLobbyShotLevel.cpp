// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLobbyShotLevel.h"
#include "Kismet/GameplayStatics.h"


void AMainLobbyShotLevel::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(GetWorld(), audioComponent);
	auto pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pc->bShowMouseCursor = true;
}
