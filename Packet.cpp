#include "Packet.h"

Packet::Packet(Buffer* buf)
{
	data = new char[buf->len+10];
	dataLen = ntohl(*((int*)(buf->base)));
	type = (PacketType)ntohl(*((int*)(buf->base + 4)));
	printf("dataLen=%d, type=%d\n", dataLen, type);
	memcpy(data, buf->base + 8, dataLen);
}

Packet::Packet(PacketType t, char* s, int l):type(t)
{
	data = new char[l];
	memcpy(data, s, l);
	dataLen = l;
}

Packet::~Packet()
{
	delete[] data;
}
