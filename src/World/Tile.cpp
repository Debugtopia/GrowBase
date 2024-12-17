#include <BaseApp.h> // precompiled
#include <World/Tile.h>

#include <Items/ItemInfoManager.h>

Tile::~Tile()
{
	//
}

void Tile::SetBackground(const uint16_t& tileID)
{
	ItemInfo * pItemInfo = GetItemInfoManager()->GetItemByID(tileID);
	if (pItemInfo == NULL)
	{
		// item info is null.
		return;
	}

	m_background = tileID;
}

bool Tile::HasFlag(const uint16_t& flag) 
{ 
	return m_flags & flag; 
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

void Tile::SetForeground(const uint16_t& tileID)
{
	ItemInfo * pItemInfo = GetItemInfoManager()->GetItemByID(tileID);
	if (pItemInfo == NULL)
	{
		// item info is null.
		return;
	}

	m_foreground = tileID;
}

void Tile::ResetNeccesaryFlags()
{
	// removing all flags, then adding only the ones that don't reset when tile is broken
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