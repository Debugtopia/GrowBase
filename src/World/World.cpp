#include <BaseApp.h> // precompiled
#include <World/World.h>

World::World(const std::string& name, const uint8_t& width, const uint8_t& height)
{
	m_name = name;
	//
}

World::~World()
{
	//
}

bool World::HasBit(const int& bit)
{
	return m_bits & bit;
}

nova_str World::GetCategoryAsString()
{
	switch (m_category)
	{
	    case WORLD_CATEGORY_ADVENTURE:
			return "Adventure";
		case WORLD_CATEGORY_ART:
			return "Art";
		case WORLD_CATEGORY_FARM:
			return "Farm";
		case WORLD_CATEGORY_GAME:
			return "Game";
		case WORLD_CATEGORY_GUILD:
			return "Guild";
		case WORLD_CATEGORY_INFORMATION:
			return "Information";
		case WORLD_CATEGORY_MUSIC:
			return "Music";
		case WORLD_CATEGORY_PARKOUR:
			return "Parkour";
		case WORLD_CATEGORY_PUZZLE:
			return "Puzzle";
		case WORLD_CATEGORY_ROLEPLAY:
			return "Roleplay";
		case WORLD_CATEGORY_SHOP:
			return "Shop";
		case WORLD_CATEGORY_SOCIAL:
			return "Social";
		case WORLD_CATEGORY_STORAGE:
			return "Storage";
		case WORLD_CATEGORY_STORY:
			return "Story";
		case WORLD_CATEGORY_TRADE:
			return "Trade";
	}

	return "None";
}

int World::GetPlayersCount()
{
	return 0;
}

void World::ToggleBit(const int& bit, const bool& bSetAsActive)
{
	if (HasBit(bit) && bSetAsActive == false)
	{
		// removing bit
		m_bits &= ~bit;
	}

	if (HasBit(bit) == false && bSetAsActive)
	{
		// adding bit
		m_bits |= bit;
	}
}

