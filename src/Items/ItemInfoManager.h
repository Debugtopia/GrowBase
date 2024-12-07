#ifndef ITEMINFOMANAGER_H
#define ITEMINFOMANAGER_H
#include <vector>
#include <algorithm>

#include <Items/ItemInfo.h>
#include <Packet/GameUpdatePacket.h>

struct GrowSplice
{
	uint16_t seed1;
	uint16_t seed2;

	uint16_t result;
};

class ItemInfoManager
{
public:
	ItemInfoManager() = default;
	~ItemInfoManager();


	uint32_t GetHash() const { return m_hash; }
	GameUpdatePacket* GetUpdatePacket() const { return m_pUpdatePacket; }
	std::vector<ItemInfo*> GetItems() const { return m_items; }


	ItemInfo* GetItemByID(const uint16_t& ID);
	ItemInfo* GetItemByName(std::string fName);
	ItemInfo* CreateSeedVersionOfLastAddedItem(const uint16_t& tileID);

	//int StringToItemMaterial(const std::string& str);
	//std::string ItemMaterialToString(const int& itemFlag);

	int StringToItemType(const std::string& str);
	std::string ItemTypeToString(const int& itemType);

	int StringToTileVisualEffect(const std::string& str);
	std::string TileVisualEffectToString(const int& visualType);

	int StringToItemFlag(const std::string& str);
	std::string ItemFlagToString(const int& itemFlag);

	int StringToTileStorage(const std::string& str);
	std::string TileStorageToString(const int& tileStorage);

	int StringToTileCollision(const std::string& str);
	std::string TileCollisionToString(const int& tileCollision);

	int StringToItemFlag2(const std::string& str);
	std::string ItemFlag2ToString(const int& itemFlag);

	int StringToItemFxFlag(const std::string& str);
	std::string ItemFxFlagToString(const int& fxFlag);

	bool Load();
	bool LoadFile();

	void Serialize(const uint16_t& version);
	void DumpItemDefinitions();

private:
	std::vector<char> m_data;
	uint32_t m_hash = 0;
	uint16_t m_version = 0;
	int m_itemCount = 0;

	GameUpdatePacket* m_pUpdatePacket = NULL;

	std::vector<ItemInfo*> m_items;
	std::vector<GrowSplice> m_splices;

};

ItemInfoManager* GetItemInfoManager();

#endif ITEMINFOMANAGER_H