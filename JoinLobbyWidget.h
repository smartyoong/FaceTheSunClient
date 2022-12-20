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
	UPROPERTY(meta = (BindWidget)) class UButton* B_GoBack; // 뒤로가기 버튼
	UPROPERTY(meta = (BindWidget)) class UButton* B_Lobby; // 뒤로가기 버튼
	UPROPERTY(meta = (BindWidget)) class UVerticalBox* LobbyList; // 로비 목록을 포함할 수직 박스 버튼
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // 클릭 효과음
	class UFaceTheSunInstance* Instance; // socket
	std::vector<RoomInfo> RoomList;
public:
	UFUNCTION(BlueprintCallable) void OnGoBackClicked();
	UFUNCTION(BlueprintCallable) void OnRefreshClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void RecvLobby();
};
