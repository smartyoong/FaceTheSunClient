// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSocket.h"
#include "JoinLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API UJoinLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget)) class UButton* B_GoBack; // �ڷΰ��� ��ư
	UPROPERTY(meta = (BindWidget)) class UButton* B_Lobby; // �ڷΰ��� ��ư
	UPROPERTY(meta = (BindWidget)) class UVerticalBox* LobbyList; // �κ� ����� ������ ���� �ڽ� ��ư
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // Ŭ�� ȿ����
	class UFaceTheSunInstance* Instance; // socket
	std::vector<RoomInfo> RoomList;
public:
	UFUNCTION(BlueprintCallable) void OnGoBackClicked();
	UFUNCTION(BlueprintCallable) void OnRefreshClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void RecvLobby();
};
