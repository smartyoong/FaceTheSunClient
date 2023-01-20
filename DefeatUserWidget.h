// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefeatUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UDefeatUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget)) class UButton* B_Next;
	UFUNCTION(BlueprintCallable) void OnNextClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	UPROPERTY() class UFaceTheSunInstance* Instance;
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* BGMSound;
};
