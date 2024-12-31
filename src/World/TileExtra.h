#ifndef TILEEXTRA_H
#define TILEEXTRA_H
#include <string>
#include <array>
#include <random>
#include <chrono>
#include <vector>
#include <unordered_map>

#include <SDK/Proton/MiscUtils.h>

enum eTileExtraType : uint8_t
{
	TILE_EXTRA_TYPE_NONE,
	TILE_EXTRA_TYPE_DOOR,
	TILE_EXTRA_TYPE_SIGN,
	TILE_EXTRA_TYPE_LOCK,
	TILE_EXTRA_TYPE_TREE,
	TILE_EXTRA_TYPE_UNK_5, //nobody knows what this is(pre-release development phase)
	TILE_EXTRA_TYPE_MAILBOX,
	TILE_EXTRA_TYPE_BULLETIN,
	TILE_EXTRA_TYPE_DICE,
	TILE_EXTRA_TYPE_PROVIDER,
	TILE_EXTRA_TYPE_ACHIEVEMENT_BLOCK,
	TILE_EXTRA_TYPE_HEART_MONITOR,
	TILE_EXTRA_TYPE_DONATION_BOX,
	TILE_EXTRA_TYPE_TOY_BOX, //unhandled
	TILE_EXTRA_TYPE_MANNEQUIN,
	TILE_EXTRA_TYPE_MAGIC_EGG,
	TILE_EXTRA_TYPE_GAME_BLOCK,
	TILE_EXTRA_TYPE_GAME_GENERATOR,
	TILE_EXTRA_TYPE_XENONITE,
	TILE_EXTRA_TYPE_DRESSUP,
	TILE_EXTRA_TYPE_CRYSTAL,
	TILE_EXTRA_TYPE_BURGLAR, //unhandled
	TILE_EXTRA_TYPE_SPOTLIGHT,
	TILE_EXTRA_TYPE_DISPLAY_BLOCK,
	TILE_EXTRA_TYPE_VENDING_MACHINE,
	TILE_EXTRA_TYPE_FISHTANK,
	TILE_EXTRA_TYPE_SOLAR,
	TILE_EXTRA_TYPE_FORGE,
	TILE_EXTRA_TYPE_GIVING_TREE,
	TILE_EXTRA_TYPE_GIVING_TREE_STUMP, //unhandled
	TILE_EXTRA_TYPE_STEAM_ORGAN, //unhandled
	TILE_EXTRA_TYPE_SILKWORM,
	TILE_EXTRA_TYPE_SEWING_MACHINE,
	TILE_EXTRA_TYPE_FLAG,
	TILE_EXTRA_TYPE_LOBSTER_TRAP,
	TILE_EXTRA_TYPE_ART_CANVAS,
	TILE_EXTRA_TYPE_BATTLE_CAGE,
	TILE_EXTRA_TYPE_PET_TRAINER, //unhandled
	TILE_EXTRA_TYPE_STEAM_ENGINE,
	TILE_EXTRA_TYPE_LOCK_BOT,
	TILE_EXTRA_TYPE_BACKGROUND_WEATHER,
	TILE_EXTRA_TYPE_SPIRIT_STORAGE,
	TILE_EXTRA_TYPE_DATA_BEDROCK, //unhandled because of different serializing on every game version
	TILE_EXTRA_TYPE_DISPLAY_SHELF,
	TILE_EXTRA_TYPE_VIP_TIMER,
	TILE_EXTRA_TYPE_CHALLENGE_TIMER,
	TILE_EXTRA_TYPE_UNK_46,
	TILE_EXTRA_TYPE_FISH_MOUNT,
	TILE_EXTRA_TYPE_PORTRAIT,
	TILE_EXTRA_TYPE_STUFF_WEATHER,
	TILE_EXTRA_TYPE_FOSSIL_PREP,
	TILE_EXTRA_TYPE_DNA_MACHINE,
	TILE_EXTRA_TYPE_TRICKSTER,
	TILE_EXTRA_TYPE_CHEMTANK,
	TILE_EXTRA_TYPE_STORAGE,
	TILE_EXTRA_TYPE_OVEN,
	TILE_EXTRA_TYPE_SUPER_MUSIC,
	TILE_EXTRA_TYPE_GEIGER_CHARGER,
	TILE_EXTRA_TYPE_ADVENTURE_RESET,
	TILE_EXTRA_TYPE_TOMB_ROBBER,
	TILE_EXTRA_TYPE_FACTION, //unhandled
	TILE_EXTRA_TYPE_TRAINING_FISH_PORT,
	TILE_EXTRA_TYPE_ITEM_SUCKER,
	TILE_EXTRA_TYPE_ROBOT,
	TILE_EXTRA_TYPE_TICKET, //unhandled
	TILE_EXTRA_TYPE_GUILD_ITEM,
	TILE_EXTRA_TYPE_STATS_BLOCK,
	TILE_EXTRA_TYPE_FIELD_NODE,  //unhandled
	TILE_EXTRA_TYPE_OUIJA_BOARD, //unhandled
	TILE_EXTRA_TYPE_AUTO_BREAK,
	TILE_EXTRA_TYPE_AUTO_HARVEST,
	TILE_EXTRA_TYPE_AUTO_HARVEST_SUCKER,
	TILE_EXTRA_TYPE_LIGHTNING_IF_ON, //unhandled
	TILE_EXTRA_TYPE_PHASED_BLOCK,
	TILE_EXTRA_TYPE_SAFE_VAULT,
	TILE_EXTRA_TYPE_PHASED_BLOCK2,
	TILE_EXTRA_TYPE_PVE_NPC, //unhandled
	TILE_EXTRA_TYPE_INFINITY_WEATHER,  //TODO: from here to end
	TILE_EXTRA_TYPE_COMPLETIONIST, //unhandled
	TILE_EXTRA_TYPE_FEEDING_BLOCK, //unhandled
	TILE_EXTRA_TYPE_KRANKENS_BLOCK,
	TILE_EXTRA_TYPE_FRIENDS_ENTRANCE
};

enum eLockFlags : uint8_t
{
	LOCK_FLAG_NONE = 0x0,
	LOCK_FLAG_AREA_LOCK = 0x1,
	LOCK_FLAG_DISABLE_MUSIC_NOTE = 0x10,
	LOCK_FLAG_INVISIBLE_MUSIC_NOTE = 0x20,
	LOCK_FLAG_IGNORE_EMPTY_AIR = 0x20,
	LOCK_FLAG_ONLY_BUILDING = 0x40,
	LOCK_FLAG_SILENCED_PLEASANTS = 0x40,
	LOCK_FLAG_ROYAL_RAINBOW = 0x80,
	LOCK_FLAG_RESTRICT_ADMIN = 0x80
};

enum eFishFlags 
{
	FISH_FLAG_NONE = 0x0,
	FISH_FLAG_GLOW_PERFECT_FISH = 0x10
};

enum eSilkwormFlags 
{
    SILKWORM_FLAG_NONE = 0,
    SILKWORM_FLAG_DEAD = 1,
    SILKWORM_FLAG_DEVIL_HORNS = 8,
    SILKWORM_FLAG_GOLDEN_HALO = 16
};

enum eSilkwormStates 
{
    SILKWORM_STATE_NONE = 0,
    SILKWORM_STATE_HAPPY = 1,
    SILKWORM_STATE_DANCING = 2,
    SILKWORM_STATE_SMILING_FLOAT = 3,
    SILKWORM_STATE_SMILING = 4,
};

struct DonationInfo
{
	int         userID;
	std::string lastName;
	
	uint16_t    itemID;
	uint8_t     count;
	std::string message;
};

struct MailInfo
{
	int                                   userID;
	std::string                           lastName;

	std::string                           message;
	std::string                           world;

	std::chrono::steady_clock::time_point sentSince;
};

struct GameConfig
{
	std::string name;

	int         teams;
	int         lives;
	int         goalScore;
	
	int         minTeams;
	int         maxTeams;

	int         killPoints;
	int         standingPoints;
	int         goalPoints;
	int         flagPoints;
	int         smashPoints;
	int         diePoints;

	int         flags;
	int         playerHits;
	int         blockHits;

	int         duration;
};

class TileExtra
{
public:
    TileExtra(const uint8_t& type)
    {
        m_type = type;
    }

    ~TileExtra() = default;

    
    // get
    uint8_t                  GetExtraType() const { return m_type; }
    virtual size_t           GetMemoryEstimated(const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5) = 0;

    // set
    void                     SetExtraType(const uint8_t& type) { m_type = type; }


    // fn
    virtual void             Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5) = 0;
    virtual void             Load(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const uint16_t& worldMapVersion = 5) = 0;

private:
    uint8_t                  m_type = 0;

};

class TileExtraDoor : public TileExtra
{
public:
    TileExtraDoor() : TileExtra(TILE_EXTRA_TYPE_DOOR) {}
    ~TileExtraDoor() = default;


    // get
    size_t           GetMemoryEstimated(const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5) override;

    // fn
    void             Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const float& fClientVersion = 2.998f, const uint16_t& worldMapVersion = 5) override;
    void             Load(uint8_t * pData, int& memOffset, const bool& bClientSide = true, const uint16_t& worldMapVersion = 5) override;

public:
    std::string      Label = "";
    std::string      Destination = "";
    std::string      UniqueID = "";
    std::string      Password = "";
    uint8_t          Flag = 0;

};

#endif TILEEXTRA_H