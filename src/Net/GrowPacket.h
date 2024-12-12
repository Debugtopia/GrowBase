#ifndef GROWPACKET_H
#define GROWPACKET_H
#include <cstdint>
#pragma warning(disable : 4200)

enum eGrowPacketType
{
	NET_GROW_PACKET_CONSOLE_OUTPUT, // sends console log to the concurrent client
	NET_GROW_PACKET_AUTHORIZE, // sends packet to logon server with authorization password & needed info to setup a sub-server on it(if authorized)
	NET_GROW_PACKET_INIT_HOST, // server replies back to the authorization packet by providing with info to init the host of the game server
	NET_GROW_PACKET_CLIENT_REDIRECT, // fowards client towards new server
	NET_GROW_PACKET_UPDATE_STATUS, // resync with other servers & update online count, etc
	NET_GROW_PACKET_DISCONNECT, // disconnect from the logon server
};

enum eGrowMessageType
{
	NET_GROW_MESSAGE_NONE,
	NET_GROW_MESSAGE_SERVER_HELLO, // used on NET_GROW_PACKET_AUTHORIZE grow packet type
	NET_GROW_MESSAGE_GROW_PACKET, // sending GrowPacket data
	NET_GROW_MESSAGE_ERROR, // let the logon server be aware of errors going on in the server
	NET_GROW_MESSAGE_TRACK // used on NET_GROW_PACKET_UPDATE_STATUS for servers tracking
};

enum eGrowPacketFlags
{
	NET_GROW_PACKET_FLAG_NONE = 0x0,
	NET_GROW_PACKET_FLAG_UPDATE = 0x1, // whether server should send resync packet
	NET_GROW_PACKET_FLAG_EXTENDED_DATA = 0x2, // extended packet data
	NET_GROW_PACKET_FLAG_MAJOR_ERROR = 0x4, // whether logon should be aware of major issue going on to kill the server temporarily or not
};

#pragma pack(push, 1)
// this packet structure is used for cross-server communication
typedef struct growpacket_t
{
	//offset 0
	uint8_t type = 0;

	//offset 1
	union
	{
		uint8_t ID = 0;
		uint8_t broadcastType;
		uint8_t eventType;
		uint8_t specialEventID;
	};

	//offset 2
	union
	{
		uint8_t itemCount = 0;
		uint8_t lostCount;
	};

	//offset 3
	union
	{
		uint8_t itemCount2 = 0;
		uint8_t gainedCount;
	};

	//offset 4
	union
	{
		int32_t serverID = 0;
		int32_t ownerID;
		int32_t broadcasterUserID;
	};

	//offset 8
	union
	{
		int32_t itemID = 0;
		int32_t latency;
		int32_t encounteredErrorCount;
	};

	//offset 12
	int32_t flags = 0;

	//offset 16
	union
	{
		float fModifier = 0;
		//
	};

	//offset 20
	union
	{
		int32_t intData = 0;
		int32_t elapsedMS;
		int32_t delayMS;
		int32_t itemsCount;
		int32_t tcpCommunicationPort;
	};

	//offset 24
	union
	{
		uint32_t intData2 = 0;
		uint32_t itemsHash;
		uint32_t enetPort;
	};

	//offset 44
	union
	{
		uint32_t intX = 0;
		uint32_t worldID;
	};

	//offset 48
	union
	{
		uint32_t intY = 0;
		uint32_t guildID;
	};

	//offset 52
	uint32_t dataLength = 0;

	//offset 56
	uint8_t data[];
} GrowPacket;

#pragma pack(pop)
#endif GROWPACKET_H