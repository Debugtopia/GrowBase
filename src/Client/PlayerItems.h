#ifndef PLAYERITEMS_H
#define PLAYERITEMS_H
#include <string>
#include <vector>

#include <Items/Defs.h>

#define MAX_BACKPACK_SIZE 396 // max slots the backpack can have
#define INITIAL_BACKPACK_SIZE 16 // the initial backpack size

//eInventoryItemFlags
#define INVENTORY_ITEM_FLAG_EQUIPPED 1 // used on clothes, it indicates that it's worn

struct PlayerInventoryItem
{
	uint16_t itemID = ITEM_ID_BLANK; // item ID of the inventory item
	uint8_t count = 0; // amount of the inventory item
	uint8_t flags = 0; // flags of the inventory item
};

class PlayerItems
{
public:
	PlayerItems();
	~PlayerItems() = default;


	// get
	int                                 GetBackpackSlots() const { return m_slots; }
	std::vector<PlayerInventoryItem>    GetItems() const { return m_items; }
	unsigned int                        GetSkinColor() const { return m_skinColor; }
	uint16_t*                           GetClothes() { return m_clothes; }
	uint16_t*                           GetTempClothes() { return m_tempClothes; }

	int                                 GetBackpackUpgradePrice();

	// set
	void SetSlots(const int& slots) { m_slots = slots; }


	// fn
	void ClearInventoryItems()
	{
		// clears all inventory items
		m_items.clear();
	}

	void RemoveClothes(const bool& bRemoveTempClothes = false)
	{
		// resets clothes
		for (int i = 0; i < NUM_CLOTHES; i++)
		{
			m_clothes[i] = ITEM_ID_BLANK;
			if (bRemoveTempClothes)
			{
				// resets temporal clothes if the option was used
				m_tempClothes[i] = ITEM_ID_BLANK;
			}
		}
	}

	void AddToInventory(const PlayerInventoryItem& inventoryItem)
	{
		m_items.push_back(inventoryItem);
	}

private:
	int                                 m_slots = INITIAL_BACKPACK_SIZE;
	std::vector<PlayerInventoryItem>    m_items;

	unsigned int                        m_skinColor = 2022356223; // player's original skin color



	uint16_t                            m_clothes[NUM_CLOTHES];
	uint16_t                            m_tempClothes[NUM_CLOTHES];

};
#endif PLAYERITEMS_H