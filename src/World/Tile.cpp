#include <BaseApp.h> // precompiled
#include <World/Tile.h>

#include <Items/ItemInfoManager.h>
#include <World/TileExtraManager.h>

Tile::~Tile()
{
	//
}

bool Tile::HasFlag(const uint16_t& flag) 
{ 
	return m_flags & flag; 
}

ItemInfo* Tile::GetItemInfo()
{
	if (m_foreground != ITEM_ID_BLANK)
	{
		// getting foreground item info
		return GetItemInfoManager()->GetItemByID(m_foreground);
	}

	// getting background item info instead
	return GetItemInfoManager()->GetItemByID(m_background);
}

size_t Tile::GetMemoryEstimated(const bool& bClientSide, const float& fClientVersion, const uint16_t& worldMapVersion)
{
	ItemInfo * pItemInfo = GetItemInfo();
	if (pItemInfo == NULL)
	{
		// item is null
		return 0;
	}
	
	size_t estimated = 0;
	estimated += sizeof(uint16_t); // foreground item ID
	estimated += sizeof(uint16_t); // background item ID
	estimated += sizeof(uint16_t); // world lock tile index
	estimated += sizeof(uint16_t); // tile flags

	if (bClientSide && m_flags & TILEFLAG_LOCKED)
	{
		// when tile is locked by an area lock, we write parent tile's index to the packet, which represents x + y * width index of the tile in the world tile map
		estimated += sizeof(uint16_t);
	}

	if (bClientSide == false)
	{
		// server side contains 2 more indexes:
		// - index > x + y * width of where the tile is located in the tile map
		// - parent > x + y * width of the world lock, if the tile is locked by an area lock instead of a world lock
		estimated += sizeof(uint16_t);
	}

	// now handling extended tile data(TileExtra)
	if (m_pExtraData != NULL && m_flags & TILEFLAG_EXTRA_DATA && GetTileExtraManager()->IsSupported(pItemInfo->type, worldMapVersion))
	{
		// if this check was passed, it means we have supported by the map version extra data to handle, otherwise client would either receive "bugged" / corrupted world data...
		// or crash entirely when entering in it
		estimated += sizeof(uint8_t); // extra tile type
		switch (pItemInfo->type)
		{
			case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
		    {
				TileExtraDoor * pExtraData = dynamic_cast<TileExtraDoor*>(m_pExtraData);
				if (pExtraData == NULL)
				{
					// extra data is null
					break;
				}

				estimated += pExtraData->GetMemoryEstimated(bClientSide, fClientVersion, worldMapVersion);
				break;
			}
		}
	}
	
	return estimated;
}

void Tile::ToggleFlag(const uint16_t& flag, const bool& bActivate)
{
	if (HasFlag(flag) && bActivate == false)
	{
		// removing the flag
		m_flags &= ~flag;
	}

	if (HasFlag(flag) == false && bActivate)
	{
		// adding the flag
		m_flags |= flag;
	}
}

bool Tile::SetForeground(const uint16_t& tileID)
{
	ItemInfo * pItemInfo = GetItemInfoManager()->GetItemByID(tileID);
	if (pItemInfo == NULL)
	{
		// item info is null.
		return false;
	}

	nova_delete(m_pExtraData); // deleting previous extra data to not leak memory
	if (GetTileExtraManager()->HasExtraData(pItemInfo->type))
	{
		// item requires extra data to world
		if (!GetTileExtraManager()->IsSupported(pItemInfo->type))
		{
			// is not supported, so we prevent placing
			return false;
		}

		m_flags |= TILEFLAG_EXTRA_DATA;
		switch (pItemInfo->type)
		{
			case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
			{
				m_pExtraData = new TileExtraDoor();
				m_pExtraData->SetExtraType(TILE_EXTRA_TYPE_DOOR);
				TileExtraDoor * pExtraData = dynamic_cast<TileExtraDoor*>(m_pExtraData);
				if (pExtraData == NULL)
				{
					// door data is null
					break;
				}

				pExtraData->Label = "";
				pExtraData->Flag = 0x3;
				pExtraData->Destination = "";
				pExtraData->UniqueID = "";
				pExtraData->Password = "";
				break;
			}
		}
	}

	m_foreground = tileID;
	return false;
}

bool Tile::SetBackground(const uint16_t& tileID)
{
	ItemInfo * pItemInfo = GetItemInfoManager()->GetItemByID(tileID);
	if (pItemInfo == NULL)
	{
		// item info is null.
		return false;
	}

	m_background = tileID;
	return true;
}

// removing all flags, then adding only the ones that don't reset when tile is broken
void Tile::ResetNeccesaryFlags()
{
	u8 new_flags = 0;
	if (m_flags & TILEFLAG_WATER)
	{
		new_flags |= TILEFLAG_WATER;
	}

	if (m_flags & TILEFLAG_FIRE)
	{
		new_flags |= TILEFLAG_FIRE;
	}

	if (m_flags & TILEFLAG_GLUE)
	{
		new_flags |= TILEFLAG_GLUE;
	}

	if (m_flags & TILEFLAG_LOCKED)
	{
		new_flags |= TILEFLAG_LOCKED;
	}

	m_flags = new_flags;
}

void Tile::ResetTileExtra()
{
	// nova_delete already has null pointer safety check, so we don't need it here
	nova_delete(m_pExtraData);
}

void Tile::Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide, const float& fClientVersion, const uint16_t& worldMapVersion)
{
    if (pData == NULL)
	{
        // data is null
		return;
	}

	MemorySerializeRaw(m_foreground, pData, memOffset, true);
	MemorySerializeRaw(m_background, pData, memOffset, true);
	MemorySerializeRaw(m_lockIndex, pData, memOffset, true);
	MemorySerializeRaw(m_flags, pData, memOffset, true);

	if (bClientSide && m_flags & TILEFLAG_LOCKED)
	{
		MemorySerializeRaw(m_parent, pData, memOffset, true);
	}

	if (bClientSide == false)
	{
		// server side info explained in GetMemoryEstimated func
		MemorySerializeRaw(m_index, pData, memOffset, true);
		MemorySerializeRaw(m_lockIndex, pData, memOffset, true);
	}

	ItemInfo * pItemInfo = GetItemInfo();
	if (pItemInfo == NULL)
	{
		// item is null
		return;
	}

	// now handling extended tile data(TileExtra)
	if (m_pExtraData != NULL && m_flags & TILEFLAG_EXTRA_DATA && GetTileExtraManager()->IsSupported(pItemInfo->type, worldMapVersion))
	{
		// if this check was passed, it means we have supported by the map version extra data to handle, otherwise client would either receive "bugged" / corrupted world data...
		// or crash entirely when entering in it
		uint8_t extra_data_type = m_pExtraData->GetExtraType();
		MemorySerializeRaw(extra_data_type, pData, memOffset, true);
		LogMsgSafe("serializing extra data for type %d", extra_data_type);

		switch (pItemInfo->type)
		{
			case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
		    {
				TileExtraDoor * pExtraData = dynamic_cast<TileExtraDoor*>(m_pExtraData);
				if (pExtraData == NULL)
				{
					// extra data is null
					break;
				}

				pExtraData->Serialize(pData, memOffset, bClientSide, fClientVersion, worldMapVersion);
				break;
			}
		}
	}
}