#ifndef GAMECLIENT_H
#define GAMECLIENT_H
#include <string>

#include <enet/enet.h>
#include <Packet/GameUpdatePacket.h>
#include <SDK/Proton/Variant.h>

#include <Client/LoginDetails.h>
#include <Client/FunctionFactory.h>
#include <Client/PlayerItems.h>

class GameClient
{
public:
	GameClient(ENetPeer * pConnectionPeer);
	~GameClient();

	// get
	ENetPeer            *GetPeer() { return m_pConnectionPeer; }
	LoginDetails        *GetLoginDetails() { return &m_loginDetails; }
	FunctionFactory     GetFunc() { return m_func; }
	PlayerItems         *GetItems() { return &m_items; }

	int                 GetUserID() const { return m_userID; }
	int                 GetOnlineID() const { return m_onlineID; }
	int                 GetAccountID() const { return m_accountID; }

	nova_str            GetName();

	// NetAvatar getters

	int                 GetNetID() const { return m_netID; }

	// returns the current skin color of the player
	// includes playmods's color modifiers(merging)
	// @note: use PlayerItems::GetSkinColor() to get the original skin color instead.
	// @note: the merged colors are never stored, they're only added to the color returned.
	unsigned int        GetSkinColor();


	// set
	void                SetOnlineID(const int& ID) { m_onlineID = ID; }
	void                SetNetID(const int& ID) { m_netID = ID; }
	void                SetAccountID(const int& ID) { m_accountID = ID; }


	// packets
	void                OnConnect();
	void                SendLog(const char* pLog, ...);
	void                SendPacketRaw(eNetMessageType eMsg, const void* pRawData, const uintmax_t& packetLen, const enet_uint32& packetFlags = ENET_PACKET_FLAG_RELIABLE);
	void                SendPacket(eNetMessageType eMsg, const std::string& textData);
	void                SendVariantPacket(VariantList variant, const int& netID = -1, const int& delayMS = 0);
	void                SendEntryFail(const bool& bResetCamera = false, const std::string& errMsg = "");


private:
	ENetPeer            *m_pConnectionPeer = NULL; // the connection peer

	LoginDetails        m_loginDetails; // logon packet's details are stored there
	FunctionFactory     m_func; // class containing variant packets
	PlayerItems         m_items; // class containing items, netavatar stuff, skin colors, inventory, ...

	int                 m_userID = 0; // non-changeable account user ID
	int                 m_onlineID = 0; // temporal online ID
	int                 m_accountID = 100; // account ID(-1 if account isn't a guest)

	// NetAvatar stuff
	int                 m_netID = 0; // temporal net ID for worlds

};

#endif GAMECLIENT_H