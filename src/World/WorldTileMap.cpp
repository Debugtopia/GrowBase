#include <BaseApp.h> // precompiled
#include <World/WorldTileMap.h>

#include <SDK/Proton/MiscUtils.h>

Tile* WorldTileMap::GetTile(const int& x, const int& y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		// tile pos is out of bounds.
		return NULL;
	}

	return &m_tiles[x + y * m_width];
}

Tile* WorldTileMap::GetTile(const uint16_t& index)
{
	if (index < 0 || index >= m_width * m_height)
	{
		// tile index is out of bounds.
		return NULL;
	}

	return &m_tiles[index];
}

Tile* WorldTileMap::GetTile(const float& x, const float& y)
{
	int tileX = static_cast<int>(x);
	int tileY = static_cast<int>(y);
	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
	{
		// tile pos is out of bounds.
		return NULL;
	}

	return &m_tiles[tileX + tileY * m_width];
}

Tile* WorldTileMap::GetTile(const CL_Vec2f& vec)
{
	int tileX = static_cast<int>(vec.X);
	int tileY = static_cast<int>(vec.Y);
	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
	{
		// tile pos is out of bounds.
		return NULL;
	}

	return &m_tiles[tileX + tileY * m_width];
}

Tile* WorldTileMap::GetTile(const CL_Vec2i& vec)
{
	if (vec.X < 0 || vec.X >= m_width || vec.Y < 0 || vec.Y >= m_height)
	{
		// tile pos is out of bounds.
		return NULL;
	}

	return &m_tiles[vec.X + vec.Y * m_width];
}

void WorldTileMap::ChooseVisualBackground(Tile* pTile, ItemInfo* pItemInfo, int& textureOffsetX, int& textureOffsetY)
{
	if (pTile == NULL || pItemInfo == NULL)
	{
		// null pointers, cant procceed.
		return;
	}

	// some hardcoded type shi
	int lut4bit[16] = { 12, 11, 15, 8, 14, 7, 13, 2, 10, 9, 6, 4, 5, 3, 1, 0 };
	int lut8bit[255];
	lut8bit[2] = 11;
	lut8bit[8] = 30;
	lut8bit[10] = 44;
	lut8bit[11] = 8;
	lut8bit[16] = 29;
	lut8bit[18] = 43;
	lut8bit[22] = 7;
	lut8bit[24] = 28;
	lut8bit[26] = 42;
	lut8bit[27] = 41;
	lut8bit[30] = 40;
	lut8bit[31] = 2;
	lut8bit[64] = 10;
	lut8bit[66] = 9;
	lut8bit[72] = 46;
	lut8bit[74] = 36;
	lut8bit[75] = 35;
	lut8bit[80] = 45;
	lut8bit[82] = 33;
	lut8bit[86] = 32;
	lut8bit[88] = 39;
	lut8bit[90] = 27;
	lut8bit[91] = 23;
	lut8bit[94] = 24;
	lut8bit[95] = 18;
	lut8bit[104] = 6;
	lut8bit[106] = 34;
	lut8bit[107] = 4;
	lut8bit[120] = 38;
	lut8bit[122] = 25;
	lut8bit[123] = 20;
	lut8bit[126] = 21;
	lut8bit[127] = 16;
	lut8bit[208] = 5;
	lut8bit[210] = 31;
	lut8bit[214] = 3;
	lut8bit[216] = 37;
	lut8bit[218] = 26;
	lut8bit[219] = 22;
	lut8bit[222] = 19;
	lut8bit[223] = 15;
	lut8bit[248] = 1;
	lut8bit[250] = 17;
	lut8bit[251] = 14;
	lut8bit[254] = 13;
	lut8bit[0] = 12;

	int x = pTile->GetIndex() % m_width;
	int y = pTile->GetIndex() / m_width;
	int i = x + y * m_width;
	switch (pItemInfo->tileStorage)
	{
	    case STORAGE_SMART_EDGE: 
	    {
		    Tile* pTileTopLeft = GetTile(x - 1, y - 1);
		    Tile* pTileTop = GetTile(x, y - 1);
		    Tile* pTileTopRight = GetTile(x + 1, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottomLeft = GetTile(x - 1, y + 1);
			Tile* pTileBottom = GetTile(x, y + 1);
			Tile* pTileBottomRight = GetTile(x + 1, y + 1);

			bool topLeft = !pTileTopLeft ? true : pTileTopLeft->GetBackground() == pItemInfo->ID ? true : pTileTopLeft->HasFlag(TILEFLAG_GLUE);
			bool top = !pTileTop ? true : pTileTop->GetBackground() == pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool topRight = !pTileTopRight ? true : pTileTopRight->GetBackground() == pItemInfo->ID ? true : pTileTopRight->HasFlag(TILEFLAG_GLUE);
			bool left = !pTileLeft ? true : pTileLeft->GetBackground() == pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetBackground() == pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);
			bool bottomLeft = !pTileBottomLeft ? true : pTileBottomLeft->GetBackground() == pItemInfo->ID ? true : pTileBottomLeft->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetBackground() == pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);
			bool bottomRight = !pTileBottomRight ? true : pTileBottomRight->GetBackground() == pItemInfo->ID ? true : pTileBottomRight->HasFlag(TILEFLAG_GLUE);


			if (!left || !top)
			{
				topLeft = false;
			}

			if (!left || !bottom)
			{
				bottomLeft = false;
			}

			if (!right || !top)
			{
				topRight = false;
			}

			if (!right || !bottom)
			{
				bottomRight = false;
			}

			int bit = 1 * topLeft + 2 * top + 4 * topRight + 8 * left + 16 * right + 32 * bottomLeft + 64 * bottom + 128 * bottomRight;

			textureOffsetX = lut8bit[bit] % 8;
			textureOffsetY = lut8bit[bit] / 8;
			break;
		}

		case STORAGE_SMART_OUTER: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetBackground() == pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool left = !pTileLeft ? true : pTileLeft->GetBackground() == pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetBackground() == pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetBackground() == pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);

			int bit = 1 * top + 2 * left + 4 * right + 8 * bottom;
			int b = lut4bit[bit];
			textureOffsetX = b % 8;
			textureOffsetY = b / 8;

			if ((pItemInfo->ID == ITEM_ID_CAVE_DIRT || pItemInfo->ID == ITEM_ID_DARK_CAVE_BACKGROUND) && pTile->HasFlag(TILEFLAG_ENABLED))
			{
				textureOffsetY += 2;
			}

			break;
		}

		case STORAGE_SMART_EDGE_VERT: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetBackground() == pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetBackground() == pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);

			if (!top && !bottom)
			{
				textureOffsetX = 3;
			}

			else if (!bottom && top)
			{
				textureOffsetX = 0;
			}

			else if (bottom && !top)
			{
				textureOffsetX = 2;
			}

			else if (top && bottom)
			{
				textureOffsetX = 1;
			}

			break;
		}

		case STORAGE_SMART_EDGE_HORIZ: 
		{
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);

			bool left = !pTileLeft ? true : pTileLeft->GetBackground() == pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetBackground() == pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);

			if (!left && !right)
			{
				textureOffsetX = 3;
			}

			else if (!left && right)
			{
				textureOffsetX = GetTile(i)->HasFlag(TILEFLAG_FLIPPED) ? 2 : 0;
			}

			else if (left && !right)
			{
				textureOffsetX = GetTile(i)->HasFlag(TILEFLAG_FLIPPED) ? 0 : 2;
			}

			else if (left && right)
			{
				textureOffsetX = 1;
			}

			break;
		}

		case STORAGE_SMART_CLING: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetBackground() == 0 ? false : pTileTop->GetBackground() != pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool left = !pTileLeft ? true : pTileLeft->GetBackground() == 0 ? false : pTileLeft->GetBackground() != pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetBackground() == 0 ? false : pTileRight->GetBackground() != pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetBackground() == 0 ? false : pTileBottom->GetBackground() != pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);

			textureOffsetX = 4;
			if (left && !top && !bottom)
			{
				textureOffsetX = 0;
			}

			if (top)
			{
				textureOffsetX = 1;
			}

			if (right && !top && !bottom)
			{
				textureOffsetX = 2;
			}

			if (bottom)
			{
				textureOffsetX = 3;
			}

			break;
		}

		case STORAGE_SMART_CLING2: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetBackground() == 0 ? false : true;
			bool left = !pTileLeft ? true : pTileLeft->GetBackground() == 0 ? false : true;
			bool right = !pTileRight ? true : pTileRight->GetBackground() == 0 ? false : true;
			bool bottom = !pTileBottom ? true : pTileBottom->GetBackground() == 0 ? false : true;

			textureOffsetX = 3;
			if (bottom)
			{
				textureOffsetX = 3;
			}

			if (top && !bottom)
			{
				textureOffsetX = 1;
			}

			if (left && !top && !bottom)
			{
				textureOffsetX = 0;
			}

			if (right && !top && !bottom)
			{
				textureOffsetX = 2;
			}

			break;
		}

		case STORAGE_RANDOM: 
		{
			textureOffsetX = Randomizer::Get(0, 3);
			break;
		}
	}
}

void WorldTileMap::ChooseVisualForeground(Tile* pTile, ItemInfo* pItemInfo, int& textureOffsetX, int& textureOffsetY)
{
	if (pTile == NULL || pItemInfo == NULL)
	{
		// null pointers, cant procceed.
		return;
	}

	// some hardcoded type shi
	int lut4bit[16] = { 12, 11, 15, 8, 14, 7, 13, 2, 10, 9, 6, 4, 5, 3, 1, 0 };
	int lut8bit[255];
	lut8bit[2] = 11;
	lut8bit[8] = 30;
	lut8bit[10] = 44;
	lut8bit[11] = 8;
	lut8bit[16] = 29;
	lut8bit[18] = 43;
	lut8bit[22] = 7;
	lut8bit[24] = 28;
	lut8bit[26] = 42;
	lut8bit[27] = 41;
	lut8bit[30] = 40;
	lut8bit[31] = 2;
	lut8bit[64] = 10;
	lut8bit[66] = 9;
	lut8bit[72] = 46;
	lut8bit[74] = 36;
	lut8bit[75] = 35;
	lut8bit[80] = 45;
	lut8bit[82] = 33;
	lut8bit[86] = 32;
	lut8bit[88] = 39;
	lut8bit[90] = 27;
	lut8bit[91] = 23;
	lut8bit[94] = 24;
	lut8bit[95] = 18;
	lut8bit[104] = 6;
	lut8bit[106] = 34;
	lut8bit[107] = 4;
	lut8bit[120] = 38;
	lut8bit[122] = 25;
	lut8bit[123] = 20;
	lut8bit[126] = 21;
	lut8bit[127] = 16;
	lut8bit[208] = 5;
	lut8bit[210] = 31;
	lut8bit[214] = 3;
	lut8bit[216] = 37;
	lut8bit[218] = 26;
	lut8bit[219] = 22;
	lut8bit[222] = 19;
	lut8bit[223] = 15;
	lut8bit[248] = 1;
	lut8bit[250] = 17;
	lut8bit[251] = 14;
	lut8bit[254] = 13;
	lut8bit[0] = 12;

	int x = pTile->GetIndex() % m_width;
	int y = pTile->GetIndex() / m_width;
	int i = x + y * m_width;
	switch (pItemInfo->tileStorage)
	{
	    case STORAGE_SMART_EDGE: 
	    {
		    Tile* pTileTopLeft = GetTile(x - 1, y - 1);
		    Tile* pTileTop = GetTile(x, y - 1);
		    Tile* pTileTopRight = GetTile(x + 1, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottomLeft = GetTile(x - 1, y + 1);
			Tile* pTileBottom = GetTile(x, y + 1);
			Tile* pTileBottomRight = GetTile(x + 1, y + 1);

			bool topLeft = !pTileTopLeft ? true : pTileTopLeft->GetForeground() == pItemInfo->ID ? true : pTileTopLeft->HasFlag(TILEFLAG_GLUE);
			bool top = !pTileTop ? true : pTileTop->GetForeground() == pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool topRight = !pTileTopRight ? true : pTileTopRight->GetForeground() == pItemInfo->ID ? true : pTileTopRight->HasFlag(TILEFLAG_GLUE);
			bool left = !pTileLeft ? true : pTileLeft->GetForeground() == pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetForeground() == pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);
			bool bottomLeft = !pTileBottomLeft ? true : pTileBottomLeft->GetForeground() == pItemInfo->ID ? true : pTileBottomLeft->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetForeground() == pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);
			bool bottomRight = !pTileBottomRight ? true : pTileBottomRight->GetForeground() == pItemInfo->ID ? true : pTileBottomRight->HasFlag(TILEFLAG_GLUE);


			if (!left || !top)
			{
				topLeft = false;
			}

			if (!left || !bottom)
			{
				bottomLeft = false;
			}

			if (!right || !top)
			{
				topRight = false;
			}

			if (!right || !bottom)
			{
				bottomRight = false;
			}

			int bit = 1 * topLeft + 2 * top + 4 * topRight + 8 * left + 16 * right + 32 * bottomLeft + 64 * bottom + 128 * bottomRight;

			textureOffsetX = lut8bit[bit] % 8;
			textureOffsetY = lut8bit[bit] / 8;
			break;
		}

		case STORAGE_SMART_OUTER: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetForeground() == pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool left = !pTileLeft ? true : pTileLeft->GetForeground() == pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetForeground() == pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetForeground() == pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);

			int bit = 1 * top + 2 * left + 4 * right + 8 * bottom;
			int b = lut4bit[bit];
			textureOffsetX = b % 8;
			textureOffsetY = b / 8;

			if ((pItemInfo->ID == ITEM_ID_CAVE_DIRT || pItemInfo->ID == ITEM_ID_DARK_CAVE_BACKGROUND) && pTile->HasFlag(TILEFLAG_ENABLED))
			{
				textureOffsetY += 2;
			}

			break;
		}

		case STORAGE_SMART_EDGE_VERT: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetForeground() == pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetForeground() == pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);

			if (!top && !bottom)
			{
				textureOffsetX = 3;
			}

			else if (!bottom && top)
			{
				textureOffsetX = 0;
			}

			else if (bottom && !top)
			{
				textureOffsetX = 2;
			}

			else if (top && bottom)
			{
				textureOffsetX = 1;
			}

			break;
		}

		case STORAGE_SMART_EDGE_HORIZ: 
		{
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);

			bool left = !pTileLeft ? true : pTileLeft->GetForeground() == pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetForeground() == pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);

			if (!left && !right)
			{
				textureOffsetX = 3;
			}

			else if (!left && right)
			{
				textureOffsetX = GetTile(i)->HasFlag(TILEFLAG_FLIPPED) ? 2 : 0;
			}

			else if (left && !right)
			{
				textureOffsetX = GetTile(i)->HasFlag(TILEFLAG_FLIPPED) ? 0 : 2;
			}

			else if (left && right)
			{
				textureOffsetX = 1;
			}

			break;
		}

		case STORAGE_SMART_CLING: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetForeground() == 0 ? false : pTileTop->GetForeground() != pItemInfo->ID ? true : pTileTop->HasFlag(TILEFLAG_GLUE);
			bool left = !pTileLeft ? true : pTileLeft->GetForeground() == 0 ? false : pTileLeft->GetForeground() != pItemInfo->ID ? true : pTileLeft->HasFlag(TILEFLAG_GLUE);
			bool right = !pTileRight ? true : pTileRight->GetForeground() == 0 ? false : pTileRight->GetForeground() != pItemInfo->ID ? true : pTileRight->HasFlag(TILEFLAG_GLUE);
			bool bottom = !pTileBottom ? true : pTileBottom->GetForeground() == 0 ? false : pTileBottom->GetForeground() != pItemInfo->ID ? true : pTileBottom->HasFlag(TILEFLAG_GLUE);

			textureOffsetX = 4;
			if (left && !top && !bottom)
			{
				textureOffsetX = 0;
			}

			if (top)
			{
				textureOffsetX = 1;
			}

			if (right && !top && !bottom)
			{
				textureOffsetX = 2;
			}

			if (bottom)
			{
				textureOffsetX = 3;
			}

			break;
		}

		case STORAGE_SMART_CLING2: 
		{
			Tile* pTileTop = GetTile(x, y - 1);
			Tile* pTileLeft = GetTile(x - 1, y);
			Tile* pTileRight = GetTile(x + 1, y);
			Tile* pTileBottom = GetTile(x, y + 1);

			bool top = !pTileTop ? true : pTileTop->GetForeground() == 0 ? false : true;
			bool left = !pTileLeft ? true : pTileLeft->GetForeground() == 0 ? false : true;
			bool right = !pTileRight ? true : pTileRight->GetForeground() == 0 ? false : true;
			bool bottom = !pTileBottom ? true : pTileBottom->GetForeground() == 0 ? false : true;

			textureOffsetX = 3;
			if (bottom)
			{
				textureOffsetX = 3;
			}

			if (top && !bottom)
			{
				textureOffsetX = 1;
			}

			if (left && !top && !bottom)
			{
				textureOffsetX = 0;
			}

			if (right && !top && !bottom)
			{
				textureOffsetX = 2;
			}

			break;
		}

		case STORAGE_RANDOM: 
		{
			textureOffsetX = Randomizer::Get(0, 3);
			break;
		}
	}
}

void WorldTileMap::GenerateTerrain(const uint8_t& terraformType, uint8_t width, uint8_t height)
{
	// fixing size
	if (width < 30)
	{
		width = 30;
	}

	if (width > 255)
	{
		// DO NOT ALLOW MORE THAN 255
		width = 255;
	}

	if (height < 30)
	{
		height = 30;
	}

	if (height > 255)
	{
		// DO NOT ALLOW MORE THAN 255
		height = 255;
	}

	m_width = width;
	m_height = height;
	m_tiles.clear();
	m_tiles.resize(width * height);
	m_spawnPoint = CL_Vec2f(0, 0);
	switch (terraformType)
	{
		case TERRATYPE_THEMONUCLEAR:
		{
			const uint16_t bedrockLayer = (width * height) - (width * 6);
			const uint16_t doorX = Randomizer::Get(2, width - 2);

			// generating terrain(supports resizing)
			for (uint8_t x = 0; x < width; ++x)
			{
				for (uint8_t y = 0; y < height; ++y)
				{
					int index = x + y * width;
					Tile tile = Tile();
					tile.SetIndex(index);

					if (index == bedrockLayer - doorX)
					{
						// main door
						tile.SetForeground(ITEM_ID_MAIN_DOOR);
						tile.SetBackground(ITEM_ID_MONOCHROMATIC_CAVE_BACKGROUND);
						m_spawnPoint = CL_Vec2f((float)(index % m_width) * 32.f + 5.f, (float)(index / m_width) * 32.f);
					}

					if (y >= bedrockLayer)
					{
						// bedrock
						tile.SetForeground(ITEM_ID_MONOCHROMATIC_BEDROCK);
						tile.SetBackground(ITEM_ID_MONOCHROMATIC_CAVE_BACKGROUND);
					}

					m_tiles[x + (y * width)] = tile;
				}
			}

			break;
		}
		case TERRATYPE_MONOCHROME:
		{
			const uint16_t bedrockLayer = (width * height) - (width * 6);
			const uint16_t bedrockHeight = bedrockLayer / width;
			const uint16_t lavaLayer = bedrockLayer - (width * 4);
			const uint16_t lavaHeight = lavaLayer / width;
			const uint16_t dirtLayer = ((height / 2) * width) - ((height / 10) * width);
			const uint16_t dirtHeight = dirtLayer / width;
			uint16_t doorIndex = dirtLayer - (std::rand() % width - 1) + 1;
			const uint16_t doorX = Randomizer::Get(2, width - 2);

			// fixing door if neccessary
			if (doorIndex >= dirtLayer)
			{
				doorIndex = dirtLayer - 1;
			}

			if (doorIndex < dirtLayer - width)
			{
				doorIndex = dirtLayer - width;
			}

			// generating terrain(supports resizing)
			int currentHeight = dirtHeight;
			int originHeight = dirtHeight;
			for (uint8_t x = 0; x < width; ++x)
			{
				for (uint8_t y = 0; y < height; ++y)
				{
					int index = x + y * width;

					Tile tile = Tile();

					if (y == currentHeight - 1)
					{
						if (x == doorX)
						{
							// main door
							int i = x + (y * width);
							tile.SetForeground(ITEM_ID_MAIN_DOOR);
							tile.SetBackground(ITEM_ID_MONOCHROMATIC_CAVE_BACKGROUND);
							m_spawnPoint = CL_Vec2f((float)(index % m_width) * 32.f + 5.f, (float)(index / m_width) * 32.f);
						}
					}

					if (y >= currentHeight)
					{
						tile.SetForeground(ITEM_ID_MONOCHROMATIC_DIRT);
						tile.SetBackground(ITEM_ID_MONOCHROMATIC_CAVE_BACKGROUND);

						if (y >= currentHeight + 1 && y < bedrockHeight && std::rand() % 80 <= 1 && x > 0 && x < width)
						{
							// rock
							tile.SetForeground(ITEM_ID_OBSIDIAN);
						}

						if (y >= lavaHeight && y < bedrockHeight && std::rand() % 7 <= 1 && x > 0 && x < width)
						{
							// lava
							tile.SetForeground(ITEM_ID_MONOCHROMATIC_LAVA);
						}

						if (y >= bedrockHeight)
						{
							// bedrock
							tile.SetForeground(ITEM_ID_MONOCHROMATIC_BEDROCK);
						}

						if (x == doorX && y == currentHeight)
						{
							tile.SetForeground(ITEM_ID_MONOCHROMATIC_BEDROCK);
							tile.SetBackground(ITEM_ID_MONOCHROMATIC_CAVE_BACKGROUND);
						}
					}

					m_tiles[index] = tile;
				}
			}

			break;
		}
		case TERRATYPE_SUNNY: default:
		{
			const uint16_t bedrockLayer = (width * height) - (width * 6);
			const uint16_t bedrockHeight = bedrockLayer / width;
			const uint16_t lavaLayer = bedrockLayer - (width * 4);
			const uint16_t lavaHeight = lavaLayer / width;
			const uint16_t dirtLayer = ((height / 2) * width) - ((height / 10) * width);
			const uint16_t dirtHeight = dirtLayer / width;
			uint16_t doorIndex = dirtLayer - (std::rand() % width - 1) + 1;
			const uint16_t doorX = Randomizer::Get(2, width - 2);

			// fixing door if neccessary
			if (doorIndex >= dirtLayer)
			{
				doorIndex = dirtLayer - 1;
			}

			if (doorIndex < dirtLayer - width)
			{
				doorIndex = dirtLayer - width;
			}

			// generating terrain(supports resizing)
			int currentHeight = dirtHeight;
			int originHeight = dirtHeight;
			for (uint8_t x = 0; x < width; ++x)
			{
				for (uint8_t y = 0; y < height; ++y)
				{
					int index = x + y * width;

					Tile tile = Tile();
					tile.SetIndex(index);

					if (y == currentHeight - 1)
					{
						if (x == doorX)
						{
							// main door
							tile.SetForeground(ITEM_ID_MAIN_DOOR);
							tile.SetBackground(ITEM_ID_CAVE_BACKGROUND);
							m_spawnPoint = CL_Vec2f((float)(index % m_width) * 32.f + 5.f, (float)(index / m_width) * 32.f);
						}
					}

					if (y >= currentHeight)
					{
						tile.SetForeground(ITEM_ID_DIRT);
						tile.SetBackground(ITEM_ID_CAVE_BACKGROUND);

						if (y >= currentHeight + 1 && y < bedrockHeight && std::rand() % 80 <= 1 && x > 0 && x < width)
						{
							// rocks
							tile.SetForeground(ITEM_ID_ROCK);
						}

						if (y >= lavaHeight && y < bedrockHeight && std::rand() % 7 <= 1 && x > 0 && x < width)
						{
							// lava
							tile.SetForeground(ITEM_ID_LAVA);
						}

						if (y >= bedrockHeight)
						{
							// bedrock
							tile.SetForeground(ITEM_ID_BEDROCK);
						}

						if (x == doorX && y == currentHeight)
						{
							tile.SetForeground(ITEM_ID_BEDROCK);
							tile.SetBackground(ITEM_ID_CAVE_BACKGROUND);
						}
					}

					m_tiles[index] = tile;
				}
			}

			break;
		}
	}
}

void WorldTileMap::RemoveAllTilesFromThisLock(Tile* pLock)
{
	if (pLock == NULL)
	{
		// lock is null
		return;
	}

	ItemInfo * pItemInfo = pLock->GetItemInfo();
	if (pItemInfo == NULL)
	{
		// item is null
		return;
	}

	const uint16_t index = pLock->GetIndex();
	for (int i = 0; i < m_tiles.size(); i++)
	{
		Tile * pTile = GetTile(i);
		if (pTile == NULL)
		{
			// tile is null
			continue;
		}

		if (!pTile->HasFlag(TILEFLAG_LOCKED) && pTile->GetParent() == 0 && pTile->GetLockIndex() == 0)
		{
			// tile is not locked
			continue;
		}

		if (pTile->GetParent() == index)
		{
			// tile is locked by this lock
			pTile->SetParent(0);
			pTile->ToggleFlag(TILEFLAG_LOCKED, false);
		}

		if (pItemInfo->lockPower == 0 && pTile->GetLockIndex() == index)
		{
			// tile is locked by a world lock
			pTile->SetLockIndex(0);
		}
	}
}

bool WorldTileMap::NeighboursThisLock(Tile* pLock, Tile* pTile, const bool& bIgnoreEmptyAir)
{
	if (pLock == NULL || pTile == NULL || pTile->GetParent() != 0)
	{
		// null pointers, or is locked already
		return false;
	}

	ItemInfo * pItemInfo = pTile->GetItemInfo();
	if (pItemInfo == NULL)
	{
		// item is null
		return false;
	}

	if (pItemInfo->type == TYPE_LOCK || pItemInfo->type == TYPE_MAIN_DOOR || pItemInfo->type == TYPE_BEDROCK)
	{
		// not lockable
		return false;
	}

	if (pItemInfo->ID == ITEM_ID_BLANK && bIgnoreEmptyAir == true)
	{
		// ignore empty air option is enabled, but tile is empty
		return false;
	}


	const uint16_t lockX = pLock->GetIndex() % m_width;
	const uint16_t lockY = pLock->GetIndex() / m_width;
	const uint16_t tileX = pTile->GetIndex() % m_width;
	const uint16_t tileY = pTile->GetIndex() / m_width;
	if (tileX == lockX && tileY == lockY)
	{
		// tile is the lock itself
		return false;
	}

	Tile* pNeighbourRight = GetTile(tileX + 1, tileY);
	if (pNeighbourRight)
	{
		if (pNeighbourRight->GetParent() == pLock->GetIndex() || (tileX + 1 == lockX && tileY == lockY))
		{
			// lockable
			return true;
		}
	}

	Tile* pNeighbourBottom = GetTile(tileX, tileY + 1);
	if (pNeighbourBottom)
	{
		if (pNeighbourBottom->GetParent() == pLock->GetIndex() || (tileX == lockX && tileY + 1 == lockY))
		{
			// lockable
			return true;
		}
	}

	Tile* pNeighbourLeft = GetTile(tileX - 1, tileY);
	if (pNeighbourLeft)
	{
		if (pNeighbourLeft->GetParent() == pLock->GetIndex() || (tileX - 1 == lockX && tileY == lockY))
		{
			// lockable
			return true;
		}
	}

	Tile* pNeighbourTop = GetTile(tileX, tileY - 1);
	if (pNeighbourTop)
	{
		if (pNeighbourTop->GetParent() == pLock->GetIndex() || (tileX == lockX && tileY - 1 == lockY))
		{
			// lockable
			return true;
		}
	}

	// cannot be locked
	return false;
}

void WorldTileMap::AddTilesThisWouldLock(Tile* pLock, const int& lockPower, const bool& bIgnoreEmptyAir)
{
	int range = 1;
	int locked = 0;
	std::vector<Tile*> lockedTiles;
	CL_Vec2i start = CL_Vec2i{ static_cast<int>(pLock->GetIndex() % m_width), static_cast<int>(pLock->GetIndex() / m_width) };
	RemoveAllTilesFromThisLock(pLock); // removing existing locked tiles

	while (locked < lockPower)
	{
		if (start.X - range > start.X + range || start.Y - range > start.Y + range)
		{
			// exceeds the range
			break;
		}

		bool bAssigned = false;
		while (true)
		{
			int32_t minDist = 99999;
			int32_t SX = -1;
			int32_t SY = -1;
			for (uint8_t tileX = start.X - range; tileX <= start.X + range; tileX++)
			{
				for (uint8_t tileY = start.Y - range; tileY <= start.Y + range; tileY++)
				{
					Tile * pCandidateTile = GetTile(tileX, tileY);
					if (pCandidateTile == NULL)
					{
						// candidate tile is null
						continue;
					}

					auto it = std::find(lockedTiles.begin(), lockedTiles.end(), pCandidateTile);
					if (it != lockedTiles.end())
					{
						// has already been checked
						continue;
					}

					if (!NeighboursThisLock(pLock, pCandidateTile, bIgnoreEmptyAir))
					{
						// cannot be locked
						continue;
					}

					const int32_t distance = std::abs(static_cast<int>(tileY - start.Y)) + std::abs(static_cast<int>(tileX - start.X));
					if (distance >= minDist)
					{
						// too far away to be locked
						continue;
					}

					minDist = distance;
					SX = tileX;
					SY = tileY;
				}
			}

			if (SX == -1 && SY == -1)
			{
				// no tile was selected
				break;
			}

			bAssigned = true;
			Tile * pSelectedTile = GetTile(SX, SY);
			if (pSelectedTile == NULL)
			{
				// selected tile is null
				continue;
			}

			// locking the tile
			pSelectedTile->SetParent(pLock->GetIndex());
			pSelectedTile->ToggleFlag(TILEFLAG_LOCKED, true);
			lockedTiles.emplace_back(pSelectedTile);
			++locked;
			if (locked >= lockPower)
			{
				// passed the power limit, stopping the lock
				break;
			}
		}

		if (!bAssigned)
		{
			// no tile was selected, empty lock?
			return;
		}

		++range;
	}
}