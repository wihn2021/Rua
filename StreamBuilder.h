#pragma once
#include <iostream>
#include <TcpServer.h>
#include "Packet.h"

class StreamBuilder: public hv::Buffer
{
public:
	StreamBuilder();
	template<class T>
	StreamBuilder& append(T i)
	{
		while ((tail + sizeof(T)) - head + 8 > maxLen)
		{
			maxLen += 64;
			char* tmp = new char[maxLen] + 8;
			std::memcpy(tmp + 8, head, tail - head);
			delete[] head;
			tail += tmp - head + 8;
			head = tmp + 8;
		}
		*((T*)tail) = toBigEndian<T>(i);
		tail += sizeof(T);
		return *this;
	}

	StreamBuilder& appendString(const char* c)
	{
		unsigned short cLen = strlen(c);
		append<unsigned short>(cLen);
		if (tail - head + cLen + 8 > maxLen)
		{
			maxLen += cLen;
			char* tmp = new char[maxLen] + 8;
			std::memcpy(tmp + 8, head, tail - head);
			delete[] head;
			tail += tmp - head + 8;
			head = tmp + 8;
		}
		memcpy(tail, c, cLen);
		tail += cLen;
		return *this;
	}
	void distrib(PacketType);
	~StreamBuilder();
private:
	char* head, *tail;
	int maxLen = 110;
	template<class T>
	T toBigEndian(T i)
	{
		switch (sizeof(T))
		{
		case 2:
			return ntohs(i);
		case 4:
			return ntohl(i);
		}
		return i;
	}
};