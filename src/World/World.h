#ifndef WORLD_H
#define WORLD_H
#include <string>
#include <vector>
#include <functional>

#include <World/WorldTileMap.h>
#include <World/WorldObjectMap.h>

enum eWorldCategories : uint8_t
{
	WORLD_CATEGORY_NONE,
	WORLD_CATEGORY_ADVENTURE,
	WORLD_CATEGORY_ART,
	WORLD_CATEGORY_FARM,
	WORLD_CATEGORY_GAME,
	WORLD_CATEGORY_GUILD,
	WORLD_CATEGORY_INFORMATION,
	WORLD_CATEGORY_MUSIC,
	WORLD_CATEGORY_PARKOUR,
	WORLD_CATEGORY_PUZZLE,
	WORLD_CATEGORY_ROLEPLAY,
	WORLD_CATEGORY_SHOP,
	WORLD_CATEGORY_SOCIAL,
	WORLD_CATEGORY_STORAGE,
	WORLD_CATEGORY_STORY,
	WORLD_CATEGORY_TRADE
};

//eWorldBits
#define WORLDBIT_NONE 0x0
#define WORLDBIT_JAMMED 0x1 // when broadcasting, the world name will show as "JAMMED"
#define WORLDBIT_IMMUNE 0x2 // when a zombie tries to infect another player, he won't be able to
#define WORLDBIT_NOPUNCH 0x4 // when a player punches another player, he won't get the knockback effect
#define WORLDBIT_NOCONSUME 0x8 // when a player tries to use a consumable, he won't be able to
#define WORLDBIT_NODROP 0x10 // when a player tries to drop an item, he won't be able to
#define WORLDBIT_ANTIGRAVITY 0x20 // modifies player's gravity, jump height
#define WORLDBIT_NOGO 0x40 // activated by moderators by doing /banworld, disables ability of peasants to enter the world
#define WORLDBIT_NOLOCKS 0x80 // activated by moderators, disables ability for players to place locks inside the world
#define WORLDBIT_NORATING 0x100 // activated by moderators, disables /rate in the current world
#define WORLDBIT_NOWAR 0x200 // when a player tries to use balloon while balloonwarz event is active, he wont be able to gain points
#define WORLDBIT_HAUNTED 0x400 // when the world has ghosts inside it
#define WORLDBIT_INACCESSIBLE 0x800 // activated by moderators by doing /nuke or /pnuke, similiar to NOGO flag

//eChemsynthColors
#define CHEMSYNTH_NONE 0
#define CHEMSYNTH_RED 916
#define CHEMSYNTH_YELLOW 924
#define CHEMSYNTH_GREEN 914
#define CHEMSYNTH_BLUE 920
#define CHEMSYNTH_PINK 918

#define WORLD_DEFAULT_WIDTH 100
#define WORLD_DEFAULT_HEIGHT 60
#define WORLD_MAX_NPCS 255 // how many npcs a world can have inside

// fowarded definitions
class GameClient;

class World
{
public:
	World(const std::string& name, const uint8_t& width = WORLD_DEFAULT_WIDTH, const uint8_t& height = WORLD_DEFAULT_HEIGHT);
	~World();


	// broadcasting
	void Broadcast(std::function<void(int, GameClient*)> fCall);
	void Broadcast(std::function<void(GameClient*)> fCall);


	// get
	int                               GetID() const { return m_ID; }
	int                               GetNetID(const bool& bIncrease = false) { return bIncrease ? m_netID++ : m_netID; }
	std::string                       GetName() const { return m_name; }
	uint16_t                          GetMapVersion() const { return m_mapVersion; }
	int                               GetBits() const { return m_bits; }
	bool                              HasBit(const int& bit);

	int                               GetActiveWeather() const { return m_activeWeather; }
	int                               GetBaseWeather() const { return m_baseWeather; }
	int                               GetWorldOwnerID() const { return m_ownerID; }
	uint16_t                          GetWorldLockIndex() const { return m_lockIndex; }
	uint8_t                           GetCategory() const { return m_category; }
	std::string                       GetCategoryAsString();
	int                               GetPlayersCount();


	WorldTileMap                      *GetWorldTileMap() { return m_pWorldTileMap; }
	WorldObjectMap                    *GetWorldObjectMap() { return m_pWorldObjectMap; }
	std::vector<GameClient*>          GetClients() { return m_clients; }

	size_t                            GetMemoryEstimated(const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5);


	// set
	void                              SetNetID(const int& netID) { m_netID = netID; }
	void                              SetName(const std::string& name) { m_name = name; }
	int                               SetBits(const int& bits) { m_bits = bits; }
	void                              ToggleBit(const int& bit, const bool& bSetAsActive = false);
	void                              SetBaseWeather(const int& weather) { m_baseWeather = weather; }
	void                              SetWeather(const int& weather) { m_activeWeather = weather; }
	void                              SetWorldOwnerID(const int& userID) { m_ownerID = userID; }
	void                              SetWorldLockIndex(const int& index) { m_lockIndex = index; }
	void                              SetCategory(const uint8_t& category) { m_category = category; }


	// fn
	void                              Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5);
    //void                              Load(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const uint16_t& worldMapVersion = 5);


	void                              AddClient(GameClient * pClient);
	void                              RemoveClient(GameClient * pClient);

	// Tile change requests
	void                              HandlePacketTileChangeRequestPunch(GameClient* pClient, GameUpdatePacket* pPacket);
	void                              HandlePacketTileChangeRequestPlace(GameClient* pClient, GameUpdatePacket* pPacket, ItemInfo* pItemInfo);
	void                              HandlePacketTileChangeRequestConsume(GameClient* pClient, GameUpdatePacket* pPacket, ItemInfo* pItemInfo);
	void                              HandlePacketTileChangeRequestWrench(GameClient* pClient, GameUpdatePacket* pPacket);

private:
	int                               m_ID = -1;
	int                               m_netID = 0;
	std::string                       m_name = "";
	uint16_t                          m_mapVersion = 5; // the version we serialize the world tiles for
	int                               m_bits = 0;

	WorldTileMap                      *m_pWorldTileMap = NULL; // world tile map
	WorldObjectMap                    *m_pWorldObjectMap = NULL; // world object map
	std::vector<GameClient*>          m_clients{};

	int                               m_activeWeather = 4; // active weather machine ID in the world
	int                               m_baseWeather = 4; // weather machine ID that it resets to after deactivating the active one

	// server side variables
	int                               m_ownerID = -1; // userID of the owner of the world
	uint16_t                          m_lockIndex = 0; // world lock's index(if the world is not locked it's 0)
	uint8_t                           m_category = WORLD_CATEGORY_NONE; // the world category(use GetCategoryAsString() to get the name of the category)

};

#endif WORLD_H