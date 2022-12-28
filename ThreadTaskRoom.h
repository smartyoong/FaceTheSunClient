// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FACETHESUN_API ThreadTaskRoom : public FRunnable
{
public:
	ThreadTaskRoom(class URoomWidget* r);
	~ThreadTaskRoom() override;
	bool Init() override;
	uint32 Run() override;
	void Exit() override;
	void Stop() override;

private:
	// ¾²·¹µå
	FRunnableThread* Thread;
	class URoomWidget* room;
	bool IsRun = true;
};