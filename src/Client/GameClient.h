#ifndef GAMECLIENT_H
#define GAMECLIENT_H
#include <string>

#include <enet/enet.h>
#include <Packet/GameUpdatePacket.h>

#include <SDK/Proton/Variant.h>
#include <SDK/Proton/Math.h>

#include <Client/LoginDetails.h>
#include <Client/VariantSender.h>
#include <Client/PlayerItems.h>

enum eCharacterStateFlags
{
	CHARACTER_STATE_FLAG_NONE = 0x0,
	CHARACTER_STATE_FLAG_NOCLIP = 0x1,
	CHARACTER_STATE_FLAG_DOUBLE_JUMP = 0x2,
	CHARACTER_STATE_FLAG_INVISIBLE = 0x4,
	CHARACTER_STATE_FLAG_NO_HANDS = 0x8,
	CHARACTER_STATE_FLAG_NO_FACE = 0x10,
	CHARACTER_STATE_FLAG_NO_BODY = 0x20,
	CHARACTER_STATE_FLAG_DEVIL_HORNS = 0x40,
	CHARACTER_STATE_FLAG_GOLDEN_HALO = 0x80,
	CHARACTER_STATE_FLAG_RESPAWN_EFFECT = 0x100, // wtf is this shit
	CHARACTER_STATE_FLAG_IGNORE_LAVA = 0x200,
	CHARACTER_STATE_FLAG_IGNORE_SPIKES = 0x400,
	CHARACTER_STATE_FLAG_FROZEN = 0x800,
	CHARACTER_STATE_FLAG_CURSED = 0x1000,
	CHARACTER_STATE_FLAG_DUCT_TAPE = 0x2000,
	CHARACTER_STATE_FLAG_SMOKE = 0x4000,
	CHARACTER_STATE_FLAG_SHINY = 0x8000,
	CHARACTER_STATE_FLAG_ZOMBIE = 0x10000,
	CHARACTER_STATE_FLAG_SPICY = 0x20000,
	CHARACTER_STATE_FLAG_SHADOWS = 0x40000,
	CHARACTER_STATE_FLAG_IRRADIATED = 0x80000,
	CHARACTER_STATE_FLAG_SPOTLIGHT = 0x100000,
	CHARACTER_STATE_FLAG_PINEAPPLE_SKIN = 0x200000,
	CHARACTER_STATE_FLAG_PINEAPPLE_FLAG = 0x400000,
	CHARACTER_STATE_FLAG_SUPPORTER = 0x800000, // unsure about this one, but it pretty much does nothing lol
	CHARACTER_STATE_FLAG_SUPER_SUPPORTER = 0x1000000,
	CHARACTER_STATE_FLAG_PINEAPPLE_AURA = 0x2000000,
	CHARACTER_STATE_FLAG_WATER_ORB = 0x4000000,
	CHARACTER_STATE_FLAG_WET = 0x8000000,
	CHARACTER_STATE_FLAG_NEON_GUM = 0x10000000
};

enum eCharacterEffectFlags
{
	CHARACTER_EFFECT_FLAG_NONE = 0x0,
	CHARACTER_EFFECT_FLAG_WINTERFEST_CROWN_RED = 0x1,
	CHARACTER_EFFECT_FLAG_WINTERFEST_CROWN_GREEN = 0x2,
	CHARACTER_EFFECT_FLAG_WINTERFEST_CROWN_SILVER = 0x4,
	CHARACTER_EFFECT_FLAG_WINTERFEST_CROWN_GOLD = 0x8,
	CHARACTER_EFFECT_FLAG_SMALL_BODY = 0x400,
	CHARACTER_EFFECT_FLAG_MIND_CONTROL = 0x800,
	CHARACTER_EFFECT_FLAG_PINEAPPLE_CHEST = 0x1000
};

class World;
class GameClient
{
public:
	GameClient(ENetPeer * pConnectionPeer);
	~GameClient();

	// get
	ENetPeer            *GetPeer() { return m_pConnectionPeer; }
	World               *GetWorld();
	LoginDetails        *GetLoginDetails() { return &m_loginDetails; }
	PlayerItems         *GetItems() { return &m_items; }

	int                 GetUserID() const { return m_userID; }
	int                 GetOnlineID() const { return m_onlineID; }
	int                 GetAccountID() const { return m_accountID; }

	nova_str            GetNameOverride() const { return m_nameOverride; }
	nova_str            GetName();
	nova_str            GetDisplayName();
	nova_str            GetStaticDisplayName();
	nova_str            GetStaticName();
	nova_str            GetCountryState();
	nova_str            GetTitleIcon();

	size_t              GetInventoryMemoryEstimated();

	// NetAvatar getters
	int                 GetNetID() const { return m_netID; }
	CL_Vec2f            GetPosition() const { return m_vec; }
	CL_Vec2f            GetRespawnPos() const { return m_respawnVec; }
	uint8_t             GetHitPower() const { return m_hitPower; }

	nova_str            GetSpawnData(const bool& bLocalPawn = false);

	// returns the current skin color of the player
	// includes playmods's color modifiers(merging)
	// @note: use PlayerItems::GetSkinColor() to get the original skin color instead.
	// @note: the merged colors are never stored, they're only added to the color returned.
	unsigned int        GetSkinColor();


	// CharacterState getters
	uint32_t            GetHairColor() const { return m_hairColor; }
	uint32_t            GetEyesColor() const { return m_eyeColor; }
	uint32_t            GetLenColor() const { return m_lenColor; }
	float               GetGravity();
	float               GetSpeed();
	float               GetWaterSpeed() const { return m_waterSpeed; }
	float               GetAcceleration() const { return m_acceleration; }
	float               GetKnockPower() const { return m_knockPower; }
	int                 GetCharacterStateFlags() const { return m_characterStateFlags; }
	int                 GetCharacterEffectFlags() const { return m_characterEffectFlags; }
	bool                HasStateFlag(const int& flag);
	bool                HasEffectFlag(const int& flag);

	// set
	void                SetWorld(World * pWorld);
	void                SetOnlineID(const int& ID) { m_onlineID = ID; }
	void                SetAccountID(const int& ID) { m_accountID = ID; }


	void                SetNameOverride(const nova_str& nickname) { m_nameOverride = nickname; }


	// serializers
	void                SerializeInventoryData(uint8_t * pData, int& memOffset);



	// NetAvatar setters
	void                SetNetID(const int& ID) { m_netID = ID; }

	void                SetPosition(float x, float y) { m_vec = CL_Vec2f(x, y); }
	void                SetPosition(CL_Vec2f vec) { m_vec = vec; }

	void                SetHitPower(const uint8_t& power) { m_hitPower = power; }

	void                SetRespawnPos(float x, float y) { m_respawnVec = CL_Vec2f(x, y); }
	void                SetRespawnPos(CL_Vec2f vec) { m_respawnVec = vec; }


	// CharacterState setters
	void                SetHairColor(const uint32_t& color) { m_hairColor = color; }
	void                SetEyesColor(const uint32_t& color) { m_eyeColor = color; }
	void                SetLenColor(const uint32_t& color) { m_lenColor = color; }
	void                SetGravity(const float& gravity) { m_gravity = gravity; }
	void                SetSpeed(const float& speed) { m_speed = speed; }
	void                SetWaterSpeed(const float& wspeed) { m_waterSpeed = wspeed; }
	void                SetAcceleration(const float& accel) { m_acceleration = accel; }
	void                SetKnockPower(const float& power) { m_knockPower = power; }
	void                SetStateFlags(const int& flags) { m_characterStateFlags = flags; }
	void                SetEffectFlags(const int& flags) { m_characterEffectFlags = flags; }

    void                ToggleStateFlag(const int& bit, const bool& bSetAsActive = false);
	void                ToggleEffectFlag(const int& bit, const bool& bSetAsActive = false);


	// packets
	void                OnConnect();
	
	void                SendLog(const char* pLog, ...);
	void                SendPacket(eNetMessageType messageType, const std::string& textData);

	void                SendPacketRaw(eNetMessageType messageType, const void* pRawData, const uintmax_t& packetLen, const enet_uint32& packetFlags = ENET_PACKET_FLAG_RELIABLE);
	void                SendVariantPacket(VariantList variant, const int& netID = -1, const int& delayMS = 0);
	void                SendInventoryState();

	void                SendEntryFail(const bool& bResetCamera = false, const std::string& errMsg = "");
	void                SendChracterState(GameClient * pClient);
	void                UpdateClothes(GameClient * pClient, const bool& bEquipSound = false);


private:
	ENetPeer            *m_pConnectionPeer = NULL; // the connection peer
	World               *m_pWorld = NULL; // the world pointer we are located in



	LoginDetails        m_loginDetails; // logon packet's details are stored there
	PlayerItems         m_items; // class containing items, netavatar stuff, skin colors, inventory, ...



	nova_str            m_nameOverride = ""; // nickname, set by using /nick as a moderator, manages PLAYER_FLAG_UNDERCOVER bit flag



	int                 m_userID = 0; // non-changeable account user ID
	int                 m_onlineID = 0; // temporal online ID
	int                 m_accountID = 100; // account ID(-1 if account isn't a guest)



	// NetAvatar stuff
	int                 m_netID = 0; // temporal net ID for worlds
	CL_Vec2f            m_vec = CL_Vec2f(0.f, 0.f);
	CL_Vec2f            m_respawnVec = CL_Vec2f(0.f, 0.f);

	uint8_t             m_hitPower = 6; // the damage we apply to the tile upon punch, modified by player mods
	


	// CharacterState state
	uint32_t            m_hairColor = MAKE_RGBA(255, 255, 255, 255);
	uint32_t            m_eyeColor = MAKE_RGBA(255, 255, 255, 255);
	uint32_t            m_lenColor = MAKE_RGBA(0, 0, 0, 255);
	int                 m_characterStateFlags = 0;
	int                 m_characterEffectFlags = 0;

	float               m_gravity = 1000.f;
	float               m_speed = 250.f;
	float               m_waterSpeed = 125.f;
	float               m_acceleration = 1200.f;
	float               m_knockPower = 200.f;

};

#endif GAMECLIENT_H