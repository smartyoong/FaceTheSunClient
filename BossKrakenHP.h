// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossKrakenHP.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UBossKrakenHP : public UUserWidget
{
	GENERATED_BODY()
	public:
	UPROPERTY(meta = (BindWidget)) class UProgressBar* PB_KrakenHP;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaSeconds) override;
	virtual void NativeOnInitialized() override;
	class ABossKraken* Boss;
};
