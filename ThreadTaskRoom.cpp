// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadTaskRoom.h"
#include "RoomWidget.h"
#include "FaceTheSunInstance.h"

void NewCommers(PackToBuffer pb, URoomWidget* room);

ThreadTaskRoom::ThreadTaskRoom(URoomWidget* r) : room(r)
{
	Thread = FRunnableThread::Create(this, TEXT("Room Network Thread"));
}

ThreadTaskRoom::~ThreadTaskRoom()
{
	if (Thread)
	{
		Thread->Kill();
		delete Thread;
	}
}

void NewCommers(PackToBuffer pb, URoomWidget* room)
{
	room->Instance->OrderQue.push(pb);
}

bool ThreadTaskRoom::Init()
{
	IsRun = true;
	if (room != nullptr)
		return true;
	else
		return false;
}

uint32 ThreadTaskRoom::Run()
{
	PackToBuffer pb(1024);
	int err = room->Instance->GetSock().Recv(&pb);
	while (true)
	{
		if (!IsRun)
			break;
		if (err == SOCKET_ERROR)
		{
			UE_LOG(LogNet, Warning, TEXT("%d"), WSAGetLastError());
		}
		else
		{
			NewCommers(pb, room);
		}
		err = room->Instance->GetSock().Recv(&pb);
	}
	return 1;
}

void ThreadTaskRoom::Exit()
{
	
}

void ThreadTaskRoom::Stop()
{
	IsRun = false;
	PackToBuffer pb(32);
	pb << PacketID::DeleteRoom;
	room->Instance->GetSock().Send(&pb); // 루프 탈출용 send
}