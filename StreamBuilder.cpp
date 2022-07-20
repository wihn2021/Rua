#include "StreamBuilder.h"

StreamBuilder::StreamBuilder()
{
	head = tail = new char[maxLen] + 8;
}



void StreamBuilder::distrib(PacketType type)
{
	base = head - 8;
	len = tail - base;
	*(int*)base = toBigEndian((int)len - 8);
	*(int*)(base + 4) = toBigEndian((int)type);
}

StreamBuilder::~StreamBuilder()
{
}
