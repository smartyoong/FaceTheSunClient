// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSocket.h"

OnlineSocket::OnlineSocket()
{
	
}

OnlineSocket::~OnlineSocket()
{
	WSACleanup();
}

void OnlineSocket::Init()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(EXIT_FAILURE);
}

void OnlineSocket::Connect()
{
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		exit(EXIT_FAILURE);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("124.54.76.95");
	if(connect(sock,(sockaddr*)&addr,sizeof(sockaddr))==SOCKET_ERROR)
		exit(EXIT_FAILURE);
}

int OnlineSocket::Send(PackToBuffer* buf)
{
	int ret = send(sock, buf->GetBuffer(), buf->GetBufferSize(), 0);
	return ret;
}
int OnlineSocket::Recv(PackToBuffer* buf)
{
	int ret = recv(sock,const_cast<char*>(buf->GetBuffer()), buf->GetBufferSize(), 0);
	return ret;
}
void OnlineSocket::Close()
{
	closesocket(sock);
}

PackToBuffer::PackToBuffer(const int buffSize_)
{
	m_Size = buffSize_;
	m_Buffer = new char[m_Size];
}

void PackToBuffer::SetBuffer(char* buf, const int size)
{
	m_Size = size;
	m_Buffer = new char[size];
	memcpy_s(m_Buffer, size, buf, size);
}

bool PackToBuffer::CheckWriteBoundary(int offsetSize_)
{
	if (m_WritePtr + offsetSize_ >= m_Size) {
		return false;
	}
	return true;
}

bool PackToBuffer::CheckReadBoundary(int offsetSize_)
{
	if (m_ReadPtr + offsetSize_ >= m_Size) {
		return false;
	}
	return true;
}

void PackToBuffer::Serialize(const std::string& in)
{
	int size = in.size();
	WRITE(size);

	for (int i = 0; i < size; i++) {
		WRITE(in[i]);
	}
}

void PackToBuffer::DeSerialize(std::string* out)
{
	int size = 0;
	READ(&size);

	char* pTemp = new char[size + 1];
	for (int i = 0; i < size; i++) {
		READ(&pTemp[i]);
	}
	pTemp[size] = '\0';
	*out = pTemp;
	pTemp = nullptr;
}

PackToBuffer& PackToBuffer::operator<<(const int& in)
{
	WRITE(in);
	return *this;
}


PackToBuffer& PackToBuffer::operator<<(const float& in)
{
	WRITE(in);
	return *this;
}

PackToBuffer& PackToBuffer::operator<<(const double& in)
{
	WRITE(in);
	return *this;
}

PackToBuffer& PackToBuffer::operator<<(const bool& in)
{
	WRITE(in);
	return *this;
}

PackToBuffer& PackToBuffer::operator<<(const std::string& in)
{
	Serialize(in);
	return *this;
}

PackToBuffer& PackToBuffer::operator>>(int* out)
{
	READ(out);
	return *this;
}

PackToBuffer& PackToBuffer::operator>>(float* out)
{
	READ(out);
	return *this;
}

PackToBuffer& PackToBuffer::operator>>(double* out)
{
	READ(out);
	return *this;
}

PackToBuffer& PackToBuffer::operator>>(bool* out)
{
	READ(out);
	return *this;
}

PackToBuffer& PackToBuffer::operator>>(std::string* out)
{
	DeSerialize(out);
	return *this;
}

void PackToBuffer::Serialize(const RoomInfo& in)
{
	Serialize(in.RoomName);
	Serialize(in.HostName);
	Serialize(in.CurrentPlayer);
	Serialize(in.CurrentUserName);
}

void PackToBuffer::DeSerialize(RoomInfo* out)
{
	DeSerialize(&out->RoomName);
	DeSerialize(&out->HostName);
	DeSerialize(&out->CurrentPlayer);
	DeSerialize(&out->CurrentUserName);
}

/*방 목록을 전송하기 위한 벡터 특수화 기본 자료형이 아니면 벡터가 에러가 나서,,,*/
void PackToBuffer::Serialize(std::vector<RoomInfo>& in)
{
	size_t size = in.size();
	WRITE(size);
	for (int i = 0; i < size; i++) {
		Serialize(in[i]);
	}
}

void PackToBuffer::DeSerialize(std::vector<RoomInfo>* out)
{
	int size = 0;
	READ(&size);

	for (int i = 0; i < size; i++) {
		RoomInfo data;
		DeSerialize(&data);
		out->push_back(data);
	}
}