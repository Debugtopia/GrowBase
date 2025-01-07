#ifndef WORLDTILEMAP_H
#define WORLDTILEMAP_H
#include <cstdint>
#include <vector>

#include <SDK/Proton/Math.h>

#include <World/Tile.h>

enum eTerraformTypes
{
	TERRATYPE_SUNNY, // default world
	TERRATYPE_BEACH, // beach blast
	TERRATYPE_DESERT, // desert blast
	TERRATYPE_MARS, // mars blast
	TERRATYPE_JUNGLE, // jungle blast
	TERRATYPE_CAVE, // cave blast
	TERRATYPE_MONOCHROME, // monochromatic blast
	TERRATYPE_UNDERSEA, // undersea blast
	TERRATYPE_BOUNTIFUL, // bountiful blast
	TERRATYPE_THEMONUCLEAR, // thermonuclear blast
	TERRATYPE_TREASURE, // treasure blast

	NUM_TERRAFORMTYPE
};

class WorldTileMap
{
public:
	WorldTileMap()
	{
		m_width = 100;
		m_height = 60;
		m_tiles.resize(static_cast<size_t>(m_width * m_height));
	}

	WorldTileMap(const uint8_t& width = 100, const uint8_t& height = 60)
	{
		m_width = width;
		m_height = height;
		m_tiles.resize(static_cast<size_t>(m_width * m_height));
	}

	~WorldTileMap() = default;


	
	// get
	uint8_t                               GetWidth() const { return m_width; }
	uint8_t                               GetHeight() const { return m_height; }
	std::vector<Tile>                     GetTiles() const { return m_tiles; }
	CL_Vec2f                              GetSpawnPoint() const { return m_spawnPoint; }


	Tile                                  *GetTile(const int& x, const int& y);
	Tile                                  *GetTile(const uint16_t& index);
	Tile                                  *GetTile(const float& x, const float& y);
	Tile                                  *GetTile(const CL_Vec2f& vec);
	Tile                                  *GetTile(const CL_Vec2i& vec);
	size_t                                GetMemoryEstimated(const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5);


	// set
	void                                  SetSpawnPoint(const CL_Vec2f& spawn) { m_spawnPoint = spawn; }



	// fn
	void                                  Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5);
    void                                  Load(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const uint16_t& worldMapVersion = 5);

	void                                  ChooseVisualBackground(Tile* pTile, ItemInfo* pItemInfo, int& textureOffsetX, int& textureOffsetY);
	void                                  ChooseVisualForeground(Tile* pTile, ItemInfo* pItemInfo, int& textureOffsetX, int& textureOffsetY);

	void                                  GenerateTerrain(const uint8_t& terraformType = TERRATYPE_SUNNY, uint8_t width = 100, uint8_t height = 60);

	void                                  RemoveAllTilesFromThisLock(Tile* pTile);
	bool                                  NeighboursThisLock(Tile* pLock, Tile* pTile, const bool& bIgnoreEmptyAir);
	void                                  AddTilesThisWouldLock(Tile* pTile, const int& lockPower, const bool& bIgnoreEmptyAir);

private:
	uint8_t                               m_width = 100;
	uint8_t                               m_height = 60;
	std::vector<Tile>                     m_tiles;

	// server side info
	CL_Vec2f                              m_spawnPoint = CL_Vec2f(0, 0);

};

#endif WORLDTILEMAP_H