#ifndef GAMEUPDATEPACKET_H
#define GAMEUPDATEPACKET_H
#include <cstdint>
#pragma warning(disable : 4200)

#define GUP_SIZE 56

enum eGamePacketType
{
	NET_GAME_PACKET_STATE,
	NET_GAME_PACKET_CALL_FUNCTION,
	NET_GAME_PACKET_UPDATE_STATUS,
	NET_GAME_PACKET_TILE_CHANGE_REQUEST,
	NET_GAME_PACKET_SEND_MAP_DATA,
	NET_GAME_PACKET_SEND_TILE_UPDATE_DATA,
	NET_GAME_PACKET_SEND_TILE_UPDATE_DATA_MULTIPLE,
	NET_GAME_PACKET_TILE_ACTIVATE_REQUEST,
	NET_GAME_PACKET_TILE_APPLY_DAMAGE,
	NET_GAME_PACKET_SEND_INVENTORY_STATE,
	NET_GAME_PACKET_ITEM_ACTIVATE_REQUEST,
	NET_GAME_PACKET_ITEM_ACTIVATE_OBJECT_REQUEST,
	NET_GAME_PACKET_SEND_TILE_TREE_STATE,
	NET_GAME_PACKET_MODIFY_ITEM_INVENTORY,
	NET_GAME_PACKET_ITEM_CHANGE_OBJECT,
	NET_GAME_PACKET_SEND_LOCK,
	NET_GAME_PACKET_SEND_ITEM_DATABASE_DATA,
	NET_GAME_PACKET_SEND_PARTICLE_EFFECT,
	NET_GAME_PACKET_SET_ICON_STATE,
	NET_GAME_PACKET_ITEM_EFFECT,
	NET_GAME_PACKET_SET_CHARACTER_STATE,
	NET_GAME_PACKET_PING_REPLY,
	NET_GAME_PACKET_PING_REQUEST,
	NET_GAME_PACKET_GOT_PUNCHED,
	NET_GAME_PACKET_APP_CHECK_RESPONSE,
	NET_GAME_PACKET_APP_INTEGRITY_FAIL,
	NET_GAME_PACKET_DISCONNECT,
	NET_GAME_PACKET_BATTLE_JOIN,
	NET_GAME_PACKET_BATTLE_EVENT,
	NET_GAME_PACKET_USE_DOOR,
	NET_GAME_PACKET_SEND_PARENTAL,
	NET_GAME_PACKET_GONE_FISHIN,
	NET_GAME_PACKET_STEAM,
	NET_GAME_PACKET_PET_BATTLE,
	NET_GAME_PACKET_NPC,
	NET_GAME_PACKET_SPECIAL,
	NET_GAME_PACKET_SEND_PARTICLE_EFFECT_V2,
	NET_GAME_PACKET_ACTIVE_ARROW_TO_ITEM,
	NET_GAME_PACKET_SELECT_TILE_INDEX,
	NET_GAME_PACKET_SEND_PLAYER_TRIBUTE_DATA,
	NET_GAME_PACKET_FTUE_SET_ITEM_TO_QUICK_INVENTORY,
	NET_GAME_PACKET_PVE_NPC,
	NET_GAME_PACKET_PVPCARD_BATTLE,
	NET_GAME_PACKET_PVE_APPLY_PLAYER_DAMAGE,
	NET_GAME_PACKET_PVE_NPC_POSITION_UPDATE,
	NET_GAME_PACKET_SET_EXTRA_MODS,
	NET_GAME_PACKET_ON_STEP_ON_TILE_MOD
};

enum eNetMessageType
{
	NET_MESSAGE_NONE,
	NET_MESSAGE_SERVER_HELLO,
	NET_MESSAGE_GENERIC_TEXT,
	NET_MESSAGE_GAME_MESSAGE,
	NET_MESSAGE_GAME_PACKET,
	NET_MESSAGE_ERROR,
	NET_MESSAGE_TRACK,
	NET_MESSAGE_CLIENT_LOG_REQUEST,
	NET_MESSAGE_CLIENT_LOG_RESPONSE
};

enum eGamePacketFlags
{
	NET_GAME_PACKET_FLAG_NONE = 0x0,
	NET_GAME_PACKET_FLAG_UPDATE = 0x1,
	NET_GAME_PACKET_FLAG_FLYING = 0x2,
	NET_GAME_PACKET_FLAG_RESET_VISUAL_STATE = 0x4,
	NET_GAME_PACKET_FLAG_EXTENDED = 0x8, // packet contains extended data
	NET_GAME_PACKET_FLAG_FACING_LEFT = 0x10,
	NET_GAME_PACKET_FLAG_ON_SOLID = 0x20,
	NET_GAME_PACKET_FLAG_ON_FIRE_DAMAGE = 0x40,
	NET_GAME_PACKET_FLAG_ON_JUMP = 0x80,
	NET_GAME_PACKET_FLAG_ON_KILLED = 0x100,
	NET_GAME_PACKET_FLAG_ON_PUNCHED = 0x200,
	NET_GAME_PACKET_FLAG_ON_PLACED = 0x400,
	NET_GAME_PACKET_FLAG_ON_TILE_ACTION = 0x800,
	NET_GAME_PACKET_FLAG_ON_GOT_PUNCHED = 0x1000,
	NET_GAME_PACKET_FLAG_ON_RESPAWNED = 0x2000,
	NET_GAME_PACKET_FLAG_ON_COLLECT_OBJECT = 0x4000,
	NET_GAME_PACKET_FLAG_ON_TRAMPOLINE = 0x8000,
	NET_GAME_PACKET_FLAG_ON_DAMAGE = 0x10000,
	NET_GAME_PACKET_FLAG_ON_SLIDE = 0x20000,
	NET_GAME_PACKET_FLAG_ON_PARASOL = 0x40000,
	NET_GAME_PACKET_FLAG_ON_JUMP_PEAK = 0x80000,
	NET_GAME_PACKET_FLAG_ON_SWIM = 0x100000,
	NET_GAME_PACKET_FLAG_ON_WALL_HANG = 0x200000,
	NET_GAME_PACKET_FLAG_ON_PUNCH_START = 0x400000,
	NET_GAME_PACKET_FLAG_ON_PUNCH_END = 0x800000,
	NET_GAME_PACKET_FLAG_ON_PUNCH_LOAD = 0x1000000,
	NET_GAME_PACKET_FLAG_ON_PUNCH_FORCE = 0x2000000,
	NET_GAME_PACKET_FLAG_ON_OVERDAMAGE = 0x4000000,
	NET_GAME_PACKET_FLAG_UNK_0x8000000 = 0x8000000,
	NET_GAME_PACKET_FLAG_ON_ACID_DAMAGE = 0x10000000
};

#pragma pack(push, 1)

// packet structure taken from beef - thanks to kevz
struct GameUpdatePacket
{
	//offset 0
	uint8_t type = 0;

	//offset 1
	union
	{
		uint8_t objectType = 0;
		uint8_t punchID;
		uint8_t npcType;
		uint8_t npcDuration;
	};

	//offset 2
	union
	{
		uint8_t count1 = 0;
		uint8_t jumpCount;
		uint8_t buildRange;
		uint8_t npcID;
		uint8_t lostCount;
	};

	//offset 3
	union
	{
		uint8_t count2 = 0;
		uint8_t animationType;
		uint8_t punchRange;
		uint8_t npcEvent;
		uint8_t particleID;
		uint8_t gainCount;
		uint8_t roll;
		uint8_t fruits;
	};

	//offset 4
	union
	{
		int32_t netID = 0;
		int32_t ownerID;
		int32_t effectFlagsCheck;
		int32_t objectChangeType;
		int32_t particleEmitID;
		int32_t selectedTileIndex;
	};

	//offset 8
	union
	{
		int32_t item = 0;
		int32_t pingHash;
		int32_t itemNetID;
		int32_t pupilColor;
		int32_t targetNetID;
		int32_t tilesLength;
	};

	//offset 12
	union
	{
		int32_t flags = 0;
		int32_t effectAltFlags;
	};

	//offset 16
	union
	{
		float fVar = 0;
		float waterSpeed;
		float objectAltCount;
	};

	//offset 20
	union
	{
		int32_t intData = 0;
		int32_t pingItem;
		int32_t elapsedMS;
		int32_t delay;
		int32_t tileDamage;
		int32_t itemID;
		int32_t itemSpeed;
		int32_t effectFlags;
		int32_t objectID;
		int32_t hash;
		int32_t verifyPos;
		int32_t clientHackType;
		int32_t decompressedSize;
	};

	//offset 24
	union
	{
		float vecX = 0;
		float posX;
		float accel;
		float punchRangeIn;
	};

	//offset 28
	union
	{
		float vecY = 0;
		float posY;
		float buildRangeIn;
		float punchStrength;
	};

	//offset 32
	union
	{
		float vec2X = 0;
		float destX;
		float gravityIn;
		float speedOut;
		float velocityX;
		float particleSize;
		float pos2X;
		int hackType;
	};

	//offset 36
	union
	{
		float vec2Y = 0;
		float destY;
		float speedIn;
		float gravityOut;
		float velocityY;
		float particleAltID;
		float pos2Y;
		int hackType2;
	};

	//offset 40
	union
	{
		float particleRotation = 0;
		float npcSpeed;
		float npcVariable;
		float notRemovingItem;
	};

	//offset 44
	union
	{
		uint32_t intX = 0;
		uint32_t tilePosX;
		uint32_t itemAltID;
		uint32_t hairColor;
		uint32_t npcTargetID;
		uint32_t gameBattleValue;
		uint32_t gameBattleTeam;
	};

	//offset 48
	union
	{
		uint32_t intY = 0;
		uint32_t tilePosY;
		uint32_t itemCount;
		uint32_t eyeColor;
		uint32_t particleSizeAlt;
		uint32_t gameBattleFlags;
		uint32_t gameBattlePoints;
	};

	//offset 52
	uint32_t dataLength = 0;

	//offset 56
	uint8_t data[];
};

#pragma pack(pop)

static uint8_t * GetExtendedDataPointerFromTankPacket(GameUpdatePacket packet)
{
	if (packet.data == NULL)
	{
		return NULL;
	}

	return packet.data;
}

#endif GAMEUPDATEPACKET_H