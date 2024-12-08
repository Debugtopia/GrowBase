#ifndef GAMECLIENT_H
#define GAMECLIENT_H
#include <string>

#include <enet/enet.h>
#include <Packet/GameUpdatePacket.h>
#include <SDK/Proton/Variant.h>

#include <Client/LoginDetails.h>

class GameClient
{
public:
	GameClient(ENetPeer * pConnectionPeer);
	~GameClient();

	// get
	ENetPeer            *GetPeer() { return m_pConnectionPeer; }
	LoginDetails        *GetLoginDetails() { return &m_loginDetails; }


	int                 GetUserID() const { return m_userID; }
	int                 GetOnlineID() const { return m_onlineID; }
	int                 GetNetID() const { return m_netID; }


	// set
	void                SetOnlineID(const int& ID) { m_onlineID = ID; }
	void                SetNetID(const int& ID) { m_netID = ID; }


	// packets
	void                OnConnect();
	void                SendLog(const char* pLog, ...);
	void                SendPacketRaw(eNetMessageType eMsg, const void* pRawData, const uintmax_t& packetLen, const enet_uint32& packetFlags = ENET_PACKET_FLAG_RELIABLE);
	void                SendPacket(eNetMessageType eMsg, const std::string& textData);
	void                SendVariantPacket(VariantList variant, const int& netID = -1, const int& delayMS = 0);
	void                SendEntryFail(const bool& bResetCamera = false, const std::string& errMsg = "");

private:
	ENetPeer            *m_pConnectionPeer = NULL;

	LoginDetails        m_loginDetails;

	int                 m_userID = 0;
	int                 m_onlineID = 0;
	int                 m_netID = 0;
};

#endif GAMECLIENT_H