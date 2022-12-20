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
enum PacketID : int // ��Ŷ ó�� ���
{
	TryLogIn = 0, // �α��� �õ�
	LogInResult = 1, // �α��� ���
	AskLobby = 2, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ���� (�κ� ��� ��û)
	SendLobby = 3, // �κ� ��� ����
	AskToRoom = 4, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ���� (�κ񿡼� ������ �����)
	SendRoomInfo = 5, // �� ����� ���� ���� (������ �ִ� ����� ��)
	AskShop = 6, // ��û�� �����°��̱� ������ ���� ��Ŷ�� �ʿ���� (���� ���� ��û)
	SendShop = 7, // ���� ����
	PurchaseItem = 8, // ������ ���� ��û
	SellItem = 9, // �Ǹ�
	GameStart = 10, // ���� ���� ��¼�� �� ����ú��� ���������� ��ȯ��ų���� �����Ƿ� �Ȼ���Ҽ��� ����
	SendGameResult = 11, // ���� ��� ����
	RecvGameResult = 12, // ���� ��� ���� ��� ����
	TrySignIn = 13, // ȸ�� ����
	SignInResult = 14, // ȸ������ ���
	TryID = 15, // ID �ߺ� üũ
	IDResult = 16, // ID �ߺ� ���
	AskCreateRoom = 17, // �����
	CreateRoomResult = 18, // ����� ��� (���⼭���� ���������� ��ȯ�� �� ����)
	DeleteRoom = 19, // �� ����
	SomeBodyJoin = 20, // ������ �� ����� ���� �����鿡�� ����
	SendChat = 21, // �濡�� ä�� ������
	RecvChat = 22 // ������ ���� ä�� �ޱ�
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
	// string , vector .. �� �ʿ��� STL���� �˾Ƽ� ����ֱ�
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
	void Serialize(std::vector<RoomInfo>& in); // Ư��ȭ
	void DeSerialize(std::vector<RoomInfo>* out); // Ư��ȭ
public:
	// operator ����� ���ΰ�?
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
	// ���۰� ����á�� �� false ��ȯ
	bool CheckWriteBoundary(int offsetSize_);
	bool CheckReadBoundary(int offsetSize_);
	// �޸� ���� ���� ���� 
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