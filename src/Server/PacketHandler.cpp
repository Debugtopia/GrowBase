#include <BaseApp.h> // precompiled
#include <Server/PacketHandler.h>

#include <Client/GameClient.h>

// text packets
#include <Packet/Client/LogonPacketListener.h>

PacketHandler g_handler;
PacketHandler* GetPacketHandler() { return &g_handler; }

PacketHandler::PacketHandler(ENetHost* pHost)
{
	m_pHost = pHost;
}

void PacketHandler::HandleIncomingClientPacket(ENetPeer* pConnectionPeer, ENetPacket* pPacket)
{
	if (pConnectionPeer == NULL || pPacket == NULL || pPacket->dataLength < 4)
	{
		// connection peer may be null
		// packet may be null
		// packet may not have the message type header inside it

		// in this case we cannot procceed handling the incoming packet
		return;
	}

	// message type is the header of the packet the growtopia client sends us, without it, we cannot define what packet is the client trying to send
	int msgType = *(int*)pPacket->data;
	switch (msgType)
	{
		case NET_MESSAGE_GENERIC_TEXT:
		{
		    // the genertic text message type is a text packet the client sends
		    // most common action text packets sent from the client use this msg type
		    // such as: action|enter_game; action|refresh_items_data; action|drop; action|trash; action|growid; etc...
			if (pConnectionPeer->data == NULL || pPacket->dataLength > 1024)
			{
				// game client may be null
				// packet may exceed the lengths we support

				// in this case we cannot procceed handling the incoming packet
				break;
			}

			memset(pPacket->data + pPacket->dataLength - 1, 0, 1);
			// this is the text packet data the client sends
			std::string textPacket = (const char*)pPacket->data + 4;
			GameClient *pClient = (GameClient*)pConnectionPeer->data;
			if (pClient == NULL)
			{
				// game client is null
				break;
			}

			if (textPacket.starts_with("requestedName"))
			{
				// this is the text packet guest(non-registered) accounts send when logging in
				GrowPacketsListener::OnHandleGuestLogon(pClient, textPacket);
				break;
			}

			if (textPacket.starts_with("tankIDName"))
			{
				// this is the text packet registered accounts send when logging in
				GrowPacketsListener::OnHandleGrowIDLogon(pClient, textPacket);
				break;
			}

			if (textPacket.starts_with("protocol|") && textPacket.find("ltoken|") != std::string::npos)
			{
				// in 4.61 version, ubisoft decided to make token-based login system to support external logins such as steam, google
				// the ltoken is the login token containing the login packet info
				// ^ the token is cyphered with base64, so to get the login packet info we must decypher it

				GrowPacketsListener::OnHandleTokenLogon(pClient, textPacket);
				break;
			}

			break;
		}
	}
}