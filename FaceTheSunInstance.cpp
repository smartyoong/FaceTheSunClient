// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceTheSunInstance.h"


void UFaceTheSunInstance::InitSock()
{
	sock.Init();
}

void UFaceTheSunInstance::DeleteSock()
{
}

OnlineSocket UFaceTheSunInstance::GetSock()
{
	return sock;
}

void UFaceTheSunInstance::SetCharacterName(FText Name)
{
	ID = Name;
}

FText UFaceTheSunInstance::GetCharacterName()
{
	return ID;
}