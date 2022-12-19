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
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // Ŭ�� ȿ����
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class URoomWidget>UI_Room; // �� �������Ʈ
	UPROPERTY() class URoomWidget* RoomWidget;
	UPROPERTY() class UFaceTheSunInstance* Instance; // ���� �� ��Ÿ������
public:
	virtual void NativeOnInitialized() override;
	UFUNCTION(BlueprintCallable) void OnGoBackClicked();
	UFUNCTION(BlueprintCallable) void OnCreateClicked();
};
