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
	UPROPERTY(meta = (BindWidget)) class UButton* B_Join; // 뒤로가기 버튼
	UPROPERTY(meta = (BindWidget)) class UListView* LobbyList; // 로비 목록을 포함할 수직 박스 버튼
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // 클릭 효과음
	class UFaceTheSunInstance* Instance; // socket
	std::vector<RoomInfo> RoomList; // 방정보 받아 오기용
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class ULoobyItemUI>UI_LobbyItem; // item
	UPROPERTY() class ULoobyItemUI* Lobby; // item pointer
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class URoomWidget>UI_Room; // 방 블루프린트
	UPROPERTY() class URoomWidget* RoomWidget;
public:
	UFUNCTION(BlueprintCallable) void OnGoBackClicked();
	UFUNCTION(BlueprintCallable) void OnRefreshClicked();
	UFUNCTION(BlueprintCallable) void OnJoinClicked();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	void RecvLobby();
};
