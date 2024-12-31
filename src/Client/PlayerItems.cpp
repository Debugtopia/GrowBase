#include <BaseApp.h> // precompiled

#include <Client/PlayerItems.h>

PlayerItems::PlayerItems()
{
	for (int i = 0; i < NUM_CLOTHES; i++)
	{
		m_clothes[i] = ITEM_ID_BLANK;
		m_tempClothes[i] = ITEM_ID_BLANK;
	}

	PlayerInventoryItem fist;
	fist.itemID = ITEM_ID_FIST;
	fist.count = 1;

	PlayerInventoryItem wrench;
	wrench.itemID = ITEM_ID_WRENCH;
	wrench.count = 1;

	m_items.emplace_back(fist);
	m_items.emplace_back(wrench);
}

uint16_t PlayerItems::GetCloth(const uint8_t& bodyPart, const bool& bTempCloth)
{
	if (bodyPart < 0 || bodyPart > NUM_CLOTHES)
	{
		return ITEM_ID_BLANK;
	}

	if (bTempCloth)
	{
		return m_tempClothes[bodyPart];
	}

	return m_clothes[bodyPart];
}

uint16_t PlayerItems::GetTempCloth(const uint8_t& bodyPart)
{
	if (bodyPart < 0 || bodyPart > NUM_CLOTHES)
	{
		return ITEM_ID_BLANK;
	}

	return m_tempClothes[bodyPart];
}

int PlayerItems::GetBackpackUpgradePrice()
{
	switch (m_slots)
	{
	    case 16: return 100;
		case 26: return 200;
		case 36: return 500;
		case 46: return 1000;
		case 56: return 1700;
		case 66: return 2600;
		case 76: return 3700;
		case 86: return 5000;
		case 96: return 6500;
		case 106: return 8200;
		case 116: return 10100;
		case 126: return 12200;
		case 136: return 14500;
		case 146: return 17000;
		case 156: return 19700;
		case 166: return 22600;
		case 176: return 25700;
		case 186: return 29000;
		case 196: return 32500;
		case 206: return 36200;
		case 216: return 40100;
		case 226: return 44200;
		case 236: return 48500;
		case 246: return 53000;
		case 256: return 57700;
		case 266: return 62600;
		case 276: return 67700;
		case 286: return 73000;
		case 296: return 78500;
		case 306: return 84200;
		case 316: return 90100;
		case 326: return 96200;
		case 336: return 102500;
		case 346: return 109000;
		case 356: return 115700;
		case 366: return 122600;
		case 376: return 129700;
		case 386: return 137000;
	}

	return 0;
}