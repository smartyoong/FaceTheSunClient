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
	UPROPERTY(meta = (BindWidget)) class UButton* B_Join; // ���� ��ư
	UPROPERTY(meta = (BindWidget)) class UTextBlock* TB_Room; // ���̸�
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // Ŭ�� ȿ����
public:
	UFUNCTION(BlueprintCallable) void OnJoinClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void SetRoomName(struct RoomInfo info);
};
