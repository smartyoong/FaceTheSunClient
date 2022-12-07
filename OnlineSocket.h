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
	TryLogIn,
	LogInResult,
	AskLobby, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음
	SendLobby,
	AskToRoom, // 요청만 보내는것이시 때문에 따로 패킷은 필요없음
	SendRoomInfo,
	AskShop, // 요청만 보내는것이기 때문에 따로 패킷은 필요없음
	SendShop,
	PurchaseItem,
	SellItem,
	GameStart,
	SendGameResult,
	RecvGameResult,
	TrySignIn,
	SignInResult,
	TryID,
	IDResult 
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
private:
	SOCKET sock;
	SOCKADDR_IN addr; //TEXT("124.54.76.95");
	int port = 18891;
	char buffer[4096];
};