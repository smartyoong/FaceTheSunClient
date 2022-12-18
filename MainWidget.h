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
	UPROPERTY(meta = (BindWidget)) class UButton* B_Create; // 방 생성 버튼
	UPROPERTY(meta = (BindWidget)) class UButton* B_Join; //방참가 버튼
	UPROPERTY(meta = (BindWidget)) class UButton* B_Shop; //상점 버튼
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // 클릭 효과음
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UCreateLobbyWidget>UI_Create; // 방생성 UI
	UPROPERTY(EditDefaultsOnly) TSubclassOf<class UJoinLobbyWidget>UI_Join; // 방참가 UI
	//UPROPERTY(EditDefaultsOnly) TSubclassOf<class UShopWidget>UI_Shop; // 상점 UI
	UPROPERTY() class UCreateLobbyWidget* CreateLobby; // 방생성 UI 반드시 필수! UPROPERTY선언 안할경우 자동으로 삭제처리
	UPROPERTY() class UJoinLobbyWidget* JoinLobby; // 참가 UI 반드시 필수! UPROPERTY선언 안할경우 자동으로 삭제처리
	//UPROPERTY() class UShopWidget* Shop; // 상점 UI 반드시 필수! UPROPERTY선언 안할경우 자동으로 삭제처리
	class AFaceTheSunGameMode* FaceTheSunMode; // 소켓용

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable) void OnJoinLobbyClicked();
	UFUNCTION(BlueprintCallable) void OnCreateLobbyClicked();
	UFUNCTION(BlueprintCallable) void OnShopClicked();
	
};
