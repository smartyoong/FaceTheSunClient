// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FACETHESUN_API ThreadTaskRoom : public FRunnable
{
public:
	ThreadTaskRoom();
	~ThreadTaskRoom() override;
	bool Init() override;
	uint32 Run() override;
	void Exit() override;
	void SetRoomWidget(class URoomWidget* r);

private:
	// ¾²·¹µå
	FRunnableThread* Thread;
	class URoomWidget* room;
};