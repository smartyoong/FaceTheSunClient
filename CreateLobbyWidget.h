// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UCreateLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_Title; // title
	UPROPERTY(meta = (BindWidget)) class UButton* B_Create; // create
	UPROPERTY(meta = (BindWidget)) class UButton* B_GoBack; // goback
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // 클릭 효과음
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class URoomWidget>UI_Room; // 방 블루프린트
	UPROPERTY() class URoomWidget* RoomWidget;
	UPROPERTY() class UFaceTheSunInstance* Instance; // 소켓 및 기타관리용
public:
	virtual void NativeOnInitialized() override;
	UFUNCTION(BlueprintCallable) void OnGoBackClicked();
	UFUNCTION(BlueprintCallable) void OnCreateClicked();
};
