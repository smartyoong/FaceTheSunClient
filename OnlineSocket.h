// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#include<string>
#include<vector>

/**
 * 
 */
enum PacketID : int // 패킷 처리 헤더
{
	TryLogIn = 0, // 로그인 시도
	LogInResult = 1, // 로그인 결과
	AskLobby = 2, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음 (로비 목록 요청)
	SendLobby = 3, // 로비 목록 전송
	AskToRoom = 4, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음 (로비에서 방으로 입장시)
	SendRoomInfo = 5, // 방 입장시 정보 전달 (기존에 있던 사람들 등)
	AskShop = 6, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음 (상점 정보 요청)
	SendShop = 7, // 상점 정보
	PurchaseItem = 8, // 아이템 구매 요청
	SellItem = 9, // 판매
	GameStart = 10, // 게임 시작 어쩌면 방 입장시부터 리슨서버로 전환시킬수도 있으므로 안사용할수도 있음
	SendGameResult = 11, // 게임 결과 전송
	RecvGameResult = 12, // 게임 결과 적용 결과 받음
	TrySignIn = 13, // 회원 가입
	SignInResult = 14, // 회원가입 결과
	TryID = 15, // ID 중복 체크
	IDResult = 16, // ID 중복 결과
	AskCreateRoom = 17, // 방생성
	CreateRoomResult = 18, // 방생성 결과 (여기서부터 리슨서버로 전환할 수 있음)
	DeleteRoom = 19, // 방 삭제
	SomeBodyJoin = 20, // 누군가 방 입장시 기존 유저들에게 갱신
	SendChat = 21, // 방에서 채팅 보내기
	RecvChat = 22 // 누군가 보낸 채팅 받기
};

struct RoomInfo
{
	std::string RoomName;
	std::string HostName;
	int CurrentPlayer = 1;
	std::vector<const char*> CurrentUserName;
	RoomInfo() = default;
	RoomInfo(std::string Room, std::string Host)
	{
		RoomName = Room;
		HostName = Host;
		CurrentUserName.reserve(3);
		CurrentUserName.push_back(Host.c_str());
	}
};


class PackToBuffer
{
public:
	PackToBuffer(const int BufferSize);
	void SetBuffer(char* buf, const int size);
	PackToBuffer(const PackToBuffer& copy)
	{
		this->SetBuffer(copy.m_Buffer, copy.m_Size);
	}
	void operator =(const PackToBuffer& copy) {
		this->SetBuffer(copy.m_Buffer, copy.m_Size);
	}

	int GetBufferSize()const { return m_Size; }
	int GetReadDataSize()const { return m_ReadPtr; }
	int GetWriteDataSize()const { return m_WritePtr; }
	const char* GetBuffer() { return m_Buffer; }
public:
	// string , vector .. 더 필요한 STL들은 알아서 집어넣길
	void Serialize(const std::string& in);
	template<typename T>
	void Serialize(const std::vector<T>& in);
	template<typename T>
	void Serialize(const T& in);
	template<typename T>
	void DeSerialize(T* out);
	template<typename T>
	void DeSerialize(std::vector<T>* out);
	void DeSerialize(std::string* out);
	void Serialize(const RoomInfo& in);
	void DeSerialize(RoomInfo* out);
	void Serialize(std::vector<RoomInfo>& in); // 특수화
	void DeSerialize(std::vector<RoomInfo>* out); // 특수화
public:
	// operator 사용할 것인가?
	PackToBuffer& operator << (const int& in);
	PackToBuffer& operator << (const float& in);
	PackToBuffer& operator << (const double& in);
	PackToBuffer& operator << (const bool& in);
	template <typename T>
	PackToBuffer& operator << (const std::vector<T>& in);
	PackToBuffer& operator << (const std::string& in);

	PackToBuffer& operator >> (int* out);
	PackToBuffer& operator >> (float* out);
	PackToBuffer& operator >> (double* out);
	PackToBuffer& operator >> (bool* out);
	template <typename T>
	PackToBuffer& operator >> (std::vector<T>* out);
	PackToBuffer& operator >> (std::string* out);
private:
	// 버퍼가 가득찼을 때 false 반환
	bool CheckWriteBoundary(int offsetSize_);
	bool CheckReadBoundary(int offsetSize_);
	// 메모리 단일 복사 과정 
	template <typename T>
	void WRITE(const T& data);
	template <typename T>
	void READ(T* pData);
private:
	int m_Size = 0;
	int m_ReadPtr = 0;
	int m_WritePtr = 0;
	char* m_Buffer = nullptr;
};

template<typename T>
inline void PackToBuffer::Serialize(const std::vector<T>& in)
{
	size_t size = in.size();
	WRITE(size);
	for (int i = 0; i < size; i++) {
		WRITE(in[i]);
	}
}

template<typename T>
inline void PackToBuffer::Serialize(const T& in)
{
	WRITE(in);
}

template<typename T>
inline void PackToBuffer::DeSerialize(T* out)
{
	READ(out);
}

template<typename T>
inline void PackToBuffer::DeSerialize(std::vector<T>* out)
{
	int size = 0;
	READ(&size);

	for (int i = 0; i < size; i++) {
		T data;
		READ(&data);
		out->push_back(data);
	}
}

template<typename T>
inline PackToBuffer& PackToBuffer::operator<<(const std::vector<T>& in)
{
	Serialize(in);
	return *this;
}

template<typename T>
inline PackToBuffer& PackToBuffer::operator>>(std::vector<T>* out)
{
	DeSerialize(out);
	return *this;
}

template<typename T>
inline void PackToBuffer::WRITE(const T& data)
{
	int dataSize = sizeof(T);
	if (CheckWriteBoundary(dataSize)) {
		memcpy_s((void* const)(m_Buffer + m_WritePtr), sizeof(m_Buffer), (const void*)(&data),
			dataSize);
		m_WritePtr += dataSize;
	}
}

template<typename T>
inline void PackToBuffer::READ(T* pData)
{
	int dataSize = sizeof(T);
	if (CheckReadBoundary(dataSize)) {
		memcpy_s((void* const)(pData), dataSize, (const void* const)(m_Buffer + m_ReadPtr), dataSize);
		m_ReadPtr += dataSize;
	}
}

class FACETHESUN_API OnlineSocket
{
public:
	OnlineSocket();
	~OnlineSocket();
	void Connect();
	int Send(PackToBuffer* buf);
	int Recv(PackToBuffer* buf);
	void Close();
	void Init();
	SOCKET GetSock() { return sock; }
private:
	SOCKET sock;
	SOCKADDR_IN addr; //TEXT("124.54.76.95");
	int port = 18891;
	char buffer[4096];
};