// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget)) class UButton* B_Create; // �� ���� ��ư
	UPROPERTY(meta = (BindWidget)) class UButton* B_Join; //������ ��ư
	UPROPERTY(meta = (BindWidget)) class UButton* B_Shop; //���� ��ư
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // Ŭ�� ȿ����
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UCreateLobbyWidget>UI_Create; // ����� UI
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UJoinLobbyWidget>UI_Join; // ������ UI
	//UPROPERTY(EditDefaultsOnly) TSubclassOf<class UShopWidget>UI_Shop; // ���� UI
	UPROPERTY() class UCreateLobbyWidget* CreateLobby; // ����� UI �ݵ�� �ʼ�! UPROPERTY���� ���Ұ�� �ڵ����� ����ó��
	UPROPERTY() class UJoinLobbyWidget* JoinLobby; // ���� UI �ݵ�� �ʼ�! UPROPERTY���� ���Ұ�� �ڵ����� ����ó��
	//UPROPERTY() class UShopWidget* Shop; // ���� UI �ݵ�� �ʼ�! UPROPERTY���� ���Ұ�� �ڵ����� ����ó��
	class AFaceTheSunGameMode* FaceTheSunMode; // ���Ͽ�

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable) void OnJoinLobbyClicked();
	UFUNCTION(BlueprintCallable) void OnCreateLobbyClicked();
	UFUNCTION(BlueprintCallable) void OnShopClicked();
	
};
