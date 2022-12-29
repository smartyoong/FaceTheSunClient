// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FaceTheSunInstance.h"
#include "RoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class FACETHESUN_API URoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget)) class UTextBlock* TB_RoomName; // title
	UPROPERTY(meta = (BindWidget)) class UButton* B_Start; // start
	UPROPERTY(meta = (BindWidget)) class UButton* B_GoBack; // goback
	UPROPERTY(meta = (BindWidget)) class UButton* B_Send; // chat
	UPROPERTY(meta = (BindWidget)) class UVerticalBox* VB_User; // user
	UPROPERTY(meta = (BindWidget)) class UScrollBox* SB_Chat; // chat list
	UPROPERTY(meta = (BindWidget)) class UEditableText* ET_Chat; // chat edit
	UPROPERTY(EditDefaultsOnly, Category = Sound) USoundBase* ClickSound; // Ŭ�� ȿ����
	FText HostName;
	class ThreadTaskRoom* tp; //������� ���� ���ú�
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaSeconds) override;
	void AddNewUserName(PackToBuffer& pb);
	void AddChat(PackToBuffer& pb);
	void HostOut();
	void MemberOut(PackToBuffer& pb);
	UFUNCTION(BlueprintCallable) void OnStartClicked();
	UFUNCTION(BlueprintCallable) void OnSendChatClicked();
	UFUNCTION(BlueprintCallable) void OnGoBackClicked();
	UPROPERTY() class UFaceTheSunInstance* Instance; // ���� �� ��Ÿ������
};

/* new  and delete�� ������ ���� ����*/
