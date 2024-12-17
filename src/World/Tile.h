#ifndef TILE_H
#define TILE_H
#include <string>
#include <chrono>

#include <SDK/Proton/Math.h>

//eTileFlags
#define TILEFLAGS_NONE 0x0000
#define TILEFLAG_EXTRA_DATA 0x0001 // contains extended tile data(tileextra)
#define TILEFLAG_LOCKED 0x0002 // is locked by an area lock(outlined)
#define TILEFLAG_SPLICED_TREE 0x0004 // seed has been spliced with another one
#define TILEFLAG_TREE_WILL_DROP_SEED 0x0008 // seed should drop extra seed upon harvest
#define TILEFLAG_TREE 0x0010 // the tile is seed(modifies it's size and makes it display fruits)
#define TILEFLAG_FLIPPED 0x0020 // used in multi-facing, turns the tile to the other side, if possible
#define TILEFLAG_ENABLED 0x0040 // used on toggleable tiles like dragon gates, opens/closes them
#define TILEFLAG_PUBLIC 0x0080 // tile is public(lock is public or entrance is open to public)
#define TILEFLAG_EXTRA_FRAME 0x0100 // ???
#define TILEFLAG_SILENCED 0x0200 // item won't do the sounds(for example jammers)
#define TILEFLAG_WATER 0x0400 // water bucket has been used on this tile
#define TILEFLAG_GLUE 0x0800 // block glue has been used on this tile
#define TILEFLAG_FIRE 0x1000 // pocket lighter or elditch flame has been used on this tile
#define TILEFLAG_PAINTED_IN_RED 0x2000 // painted in red
#define TILEFLAG_PAINTED_IN_GREEN 0x4000 // painted in green
#define TILEFLAG_PAINTED_IN_BLUE 0x8000 // painted in blue
#define TILEFLAG_PAINTED_IN_PURPLE TILEFLAG_PAINTED_IN_RED | TILEFLAG_PAINTED_IN_BLUE // painted in purple(red + blue)
#define TILEFLAG_PAINTED_IN_CYAN TILEFLAG_PAINTED_IN_GREEN | TILEFLAG_PAINTED_IN_BLUE // painted in cyan(green + blue)
#define TILEFLAG_PAINTED_IN_YELLOW TILEFLAG_PAINTED_IN_RED | TILEFLAG_PAINTED_IN_GREEN // painted in yellow(red + green)
#define TILEFLAG_PAINTED_IN_CHARCOAL TILEFLAG_PAINTED_IN_RED | TILEFLAG_PAINTED_IN_GREEN | TILEFLAG_PAINTED_IN_BLUE // painted in yellow(red + green + black)


//eBattleGameFlags
#define GAMEBATTLEFLAG_NONE 0x0000
#define GAMEBATTLEFLAG_RESPAWN_ON_STORE 0x0001 // when u score point, you will respawn
#define GAMEBATTLEFLAG_RESET_ON_SCORE 0x0002 // when someone scores point, game resets
#define GAMEBATTLEFLAG_OWNER_PLAYS 0x0004 // whether the world owner plays in the game
#define GAMEBATTLEFLAG_LATE_JOIN 0x0008 // whether player who has entered in the world after the game has started joins in, or doesn't participate in it
#define GAMEBATTLEFLAG_SMASH_ENEMY_BLOCK 0x0010 // whether you're able to break game blocks of other teams
#define GAMEBATTLEFLAG_SMASH_OWN_BLOCK 0x0020 // whether you're able to break your team's game blocks
#define GAMEBATTLEFLAG_NO_MUSIC 0x0040 // whether the game music should be disabled
#define GAMEBATTLEFLAG_NO_LOG 0x0080 // whether the messages in the console log are disabled 
#define GAMEBATTLEFLAG_ENDLESS 0x0100 // whether game's timer is irrelevant and runs forever

class ItemInfo;
class TileExtraManager;
class Tile
{
public:
	Tile()
	{
		m_foreground = ITEM_ID_BLANK;
		m_background = ITEM_ID_BLANK;
		m_parent = 0;
		m_flags = TILEFLAGS_NONE;
	}

	Tile(const uint16_t& fg, const uint16_t& bg, const uint16_t& lockIndex, const uint16_t& flags)
	{
		m_foreground = fg;
		m_background = bg;
		m_parent = lockIndex;
		m_flags = flags;
	}

	~Tile();



	// get
	uint16_t                              GetForeground() const { return m_foreground; }
	uint16_t                              GetBackground() const { return m_background; }
	uint16_t                              GetParent() const { return m_parent; }
	uint16_t                              GetFlags() const { return m_flags; }
	uint16_t                              GetIndex() const { return m_index; }
	uint16_t                              GetLockIndex() const { return m_lockIndex; }
	uint8_t                               GetDamage() const { return m_damage; }
	bool                                  HasFlag(const uint16_t& flag);


	// set
	void                                  SetForeground(const uint16_t& tileID);
	void                                  SetBackground(const uint16_t& tileID);
	void                                  SetParent(const uint16_t& lockIndex) { m_parent = lockIndex; }
	void                                  SetFlags(const uint16_t& flags) { m_flags = flags; }
	void                                  ToggleFlag(const uint16_t& flag, const bool& bActivate = false);
	void                                  SetIndex(const uint16_t& index) { m_index = index; }
	void                                  SetDamage(const uint8_t& damage) { m_damage = damage; }

	// fn
	void                                  ResetNeccesaryFlags();

public:
	std::chrono::steady_clock::time_point m_damageTick;
	std::chrono::steady_clock::time_point m_sfxTick;
	std::chrono::steady_clock::time_point m_fishTick;

private:
	uint16_t                              m_foreground = ITEM_ID_BLANK;
	uint16_t                              m_background = ITEM_ID_BLANK;
	uint16_t                              m_parent = 0; // the index(x + y * width) of the parent lock
	uint16_t                              m_flags = TILEFLAGS_NONE;

	// server side information

	uint16_t                              m_index = 0; // the index(x + y * width) of this tile
	uint16_t                              m_lockIndex = 0; // the index(x + y * width) of the world lock
	uint8_t                               m_damage = 0; // the amount of damage, that has been applied to this tile
};

#endif TILE_H