#pragma once
#include "common.h"

enum PacketType
{
	REGISTER_CONNECTION = 161,
	TEAM_LIST = 115,
	HEART_BEAT = 108,
	SEND_CHAT = 141,
	SERVER_INFO = 106,
	START_GAME = 120,
	QUESTION = 117,
	QUESTION_RESPONCE = 118,
	
	//Client Commands
	PREREGISTER_CONNECTION = 160,
	HEART_BEAT_RESPONSE = 109,
	ADD_CHAT = 140,
	PLAYER_INFO = 110,
	DISCONNECT = 111,
	RANDY = 112,
	REDIRECT = 178,
	
	//Game Commands
	ADD_GAMECOMMAND = 20,
	TICK = 10,
	SYNC_CHECKSUM = 30,
	SYNC_CHECKSUM_RESPONCE = 31,
	SYNC = 35,
};

class Packet
{
public:
	int dataLen;
	PacketType type;
	char* data;
	Packet(Buffer*);
	Packet(PacketType, char*, int);
	~Packet();
	operator Buffer();
	std::ostream& operator<<(std::ostream&);
};

