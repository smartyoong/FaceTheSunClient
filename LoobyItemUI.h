// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoobyItemUI.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API ULoobyItemUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget)) class UButton* B_Join; // 참가 버튼
	UPROPERTY(meta = (BindWidget)) class UTextBlock* TB_Room; // 방이름
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // 클릭 효과음
public:
	UFUNCTION(BlueprintCallable) void OnJoinClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void SetRoomName(struct RoomInfo info);
};
