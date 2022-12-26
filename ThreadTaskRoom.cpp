// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadTaskRoom.h"
#include "RoomWidget.h"
#include "FaceTheSunInstance.h"

void NewCommers(PackToBuffer& pb, URoomWidget* room);

ThreadTaskRoom::ThreadTaskRoom()
{
	Thread = FRunnableThread::Create(this, TEXT("Room Network Thread"));
}

ThreadTaskRoom::~ThreadTaskRoom()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}
void ThreadTaskRoom::SetRoomWidget(URoomWidget* r)
{
	room = r;
}

void NewCommers(PackToBuffer& pb, URoomWidget* room)
{
	std::string NewUser;
	pb >> &NewUser;
	room->AddNewUserName(NewUser);
}

bool ThreadTaskRoom::Init()
{
	UE_LOG(LogNet, Warning, TEXT("Thread has been initialized"));
	if (room != nullptr)
		return true;
	else
		return false;
}

uint32 ThreadTaskRoom::Run()
{
	while (true)
	{
		int KindOfRecvCode = 0;
		PackToBuffer pb(1024);
		room->Instance->GetSock().Recv(&pb);
		pb >> &KindOfRecvCode;
		if (KindOfRecvCode == PacketID::DeleteRoom)
			break;
		switch (KindOfRecvCode)
		{
		case PacketID::SomeBodyJoin:
			NewCommers(pb, room);
			break;
		case PacketID::RecvChat:
			break;
		case PacketID::GameStart:
			break;
		default:
			break;
		}
	}
	return 1;
}

void ThreadTaskRoom::Exit()
{
	UE_LOG(LogNet, Warning, TEXT("Finish"));
}