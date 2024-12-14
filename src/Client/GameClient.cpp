#include <BaseApp.h> // precompiled

#include <Client/GameClient.h>


GameClient::GameClient(ENetPeer * pConnectionPeer)
{
	/* storing player as peer's data, it's deleted upon disconnect */
	pConnectionPeer->data = this;
	m_pConnectionPeer = pConnectionPeer;

}

GameClient::~GameClient()
{
	//
}

nova_str GameClient::GetName()
{
	if (m_accountID == -1)
	{
		return m_loginDetails.tankIDName;
	}

	return m_loginDetails.requestedName + "_" + std::to_string(m_accountID);
}

void GameClient::OnConnect()
{
	if (m_pConnectionPeer == NULL)
	{
		// peer is null
		return;
	}

	// this packet procceed the client to receive the "Logging on..." message
	// it does not contain any extra data, just a single byte and the NET_MESSAGE_SERVER_HELLO as header for client to understand it
	SendPacketRaw(NET_MESSAGE_SERVER_HELLO, NULL, 1);
}

void GameClient::SendLog(const char* pLog, ...)
{
	if (m_pConnectionPeer == NULL || m_pConnectionPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// enet connection is null or it's state is not connected to our server, therefore we cannot send the packet
		return;
	}

	va_list argsVA;
	const int logSize = 4096;
	char buffer[logSize];
	memset(buffer, 0, logSize);
	va_start(argsVA, pLog);

#ifdef WIN32
	vsnprintf_s(buffer, logSize, logSize, pLog, argsVA);
#elif defined(LINUX)
	vsnprintf(buffer, logSize, pLog, argsVA);
#else
	vsnprintf(buffer, logSize, pLog, argsVA);
#endif

	va_end(argsVA);
	SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|log\nmsg|" + std::string(buffer));
}

void GameClient::SendPacketRaw(eNetMessageType eMsg, const void* pRawData, const uintmax_t& packetLen, const enet_uint32& packetFlags)
{
	// default value of packetFlags is ENET_PACKET_FLAG_RELIABLE
	if (m_pConnectionPeer == NULL || m_pConnectionPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// enet connection is null or it's state is not connected to our server, therefore we cannot send the packet
		return;
	}

	// this is the packet we send to the game client
	ENetPacket* pClientPacket = enet_packet_create(NULL, 5 + packetLen, packetFlags);
	int offset = 0;
	if (pClientPacket == NULL)
	{
		// failed to create packet, so it would crash if we procceed writing stuff into it
		return;
	}

	// eMsg stands for header of the game client packet - every packet has that header
	nova_memcopy(pClientPacket->data, &eMsg, 4, offset);
	pClientPacket->data[packetLen + 4] = 0;
	if (pRawData != NULL)
	{
		// null check to prevent crash upon copying the packet data to the packet's data
		nova_memcopy(pClientPacket->data + 4, pRawData, packetLen, offset);
	}

	if (enet_peer_send(m_pConnectionPeer, 0, pClientPacket) != 0)
	{
		// destroying the packet incase it was not sent to the client
		enet_packet_destroy(pClientPacket);
	}
}

void GameClient::SendPacket(eNetMessageType eMsg, const std::string& textData)
{
	if (m_pConnectionPeer == NULL || m_pConnectionPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// enet connection is null or it's state is not connected to our server, therefore we cannot send the packet
		return;
	}

	// sending a text packet to the client
	SendPacketRaw(eMsg, textData.data(), textData.size(), ENET_PACKET_FLAG_RELIABLE);
}

void GameClient::SendVariantPacket(VariantList variant, const int& netID, const int& delayMS)
{
	if (m_pConnectionPeer == NULL || m_pConnectionPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// enet connection is null or it's state is not connected to our server, therefore we cannot send the packet
		return;
	}

	// default value of netID is -1
	// default value of delayMS is 0
	i32 variantPacketLen = 0;
	int offset = 0;
	u8* pVariantData = variant.SerializeToMem(&variantPacketLen);
	if (pVariantData == NULL)
	{
		// failed to serialize variant to mem
		return;
	}

	// this is the tank packet we send to the client
	GameUpdatePacket * pVariantPacket = (GameUpdatePacket *)std::malloc(sizeof(GameUpdatePacket) + variantPacketLen);
	if (pVariantPacket == NULL)
	{
		return;
	}

	pVariantPacket->type = NET_GAME_PACKET_CALL_FUNCTION;
	pVariantPacket->netID = netID;
	pVariantPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pVariantPacket->delay = delayMS;
	pVariantPacket->dataLength = (u32)variantPacketLen;
	nova_memcopy(&pVariantPacket->data, pVariantData, variantPacketLen, offset);

	SendPacketRaw(NET_MESSAGE_GAME_PACKET, pVariantPacket, sizeof(GameUpdatePacket) + pVariantPacket->dataLength);
	// freeing the packet from memory when sent to client so it won't leak memory
	free(pVariantPacket);
	free(pVariantData);
}

void GameClient::SendEntryFail(const bool& bResetCamera, const std::string& errMsg)
{
	if (m_pConnectionPeer == NULL || m_pConnectionPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// enet connection is null or it's state is not connected to our server, therefore we cannot send the packet
		return;
	}

	// default value of bResetCamera is 0/false
	// default value of errMsg is ""/empty
	if (errMsg != "")
	{
		SendLog(errMsg.c_str());
	}

	SendVariantPacket({ "OnFailedToEnterWorld" });
	if (bResetCamera)
	{
		SendVariantPacket({ "OnZoomCamera", 10000, 1000 });
		SendVariantPacket({ "OnSetFreezeState", 0 });
	}
}