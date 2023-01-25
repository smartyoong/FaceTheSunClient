// Fill out your copyright notice in the Description page of Project Settings.


#include "BossKrakenHP.h"
#include "Components/ProgressBar.h"
#include "BossKraken.h"
#include "Kismet/GameplayStatics.h"

void UBossKrakenHP::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBossKrakenHP::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBossKrakenHP::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
	Super::NativeTick(Geometry, DeltaSeconds);
}