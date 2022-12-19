// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceTheSunInstance.h"


void UFaceTheSunInstance::InitSock()
{
	NetWorkSocket.Init();
}

void UFaceTheSunInstance::DeleteSock()
{
}

void UFaceTheSunInstance::SetCharacterName(FText Name)
{
	ID = Name;
}

FText UFaceTheSunInstance::GetCharacterName()
{
	return ID;
}