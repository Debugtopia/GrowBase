#include <BaseApp.h> // precompiled
#include <Server/PacketHandler.h>

#include <Client/GameClient.h>

// text packets
#include <Packet/Client/LogonPacketListener.h>

// generic action packets
#include <Packet/Client/Generic/EnterGameListener.h>
#include <Packet/Client/Generic/JoinRequestListener.h>
#include <Packet/Client/Generic/QuitToExitListener.h>

// menu
#include <Packet/Client/Generic/Menu./GameHelperListener.h>

// tank update packets
#include <Packet/Client/Tank/StateListener.h>


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
		LogError("failed to proccess incoming packet, something went wrong!");
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
				LogError("failed to proccess incoming NET_MESSAGE_GENERIC_TEXT packet, something went wrong!(2)");
				return;
			}

			memset(pPacket->data + pPacket->dataLength - 1, 0, 1);
			// this is the text packet data the client sends
			const nova_str& text_data = (const char*)pPacket->data + 4;
			GameClient *pClient = (GameClient*)pConnectionPeer->data;
			if (pClient == NULL)
			{
				// game client is null
				LogError("failed to proccess incoming NET_MESSAGE_GENERIC_TEXT packet, client is NULL!");
				return;
			}

			printf("%s\n", text_data.c_str());
			if (text_data.starts_with("requestedName"))
			{
				// this is the text packet guest(non-registered) accounts send when logging in
				GrowPacketsListener::OnHandleGuestLogon(pClient, text_data);
				return;
			}

			if (text_data.starts_with("tankIDName"))
			{
				// this is the text packet registered accounts send when logging in
				GrowPacketsListener::OnHandleGrowIDLogon(pClient, text_data);
				return;
			}

			if (text_data.starts_with("protocol|") && text_data.find("ltoken|") != std::string::npos)
			{
				// in 4.61 version, ubisoft decided to make token-based login system to support external logins such as steam, google
				// the ltoken is the login token containing the login packet info
				// ^ the token is cyphered with base64, so to get the login packet info we must decypher it

				GrowPacketsListener::OnHandleTokenLogon(pClient, text_data);
				return;
			}

			if (text_data.starts_with("action|refresh_item_data"))
			{
				if (pClient == NULL)
				{
					// failed to procceed to update items data because client is null
					return;
				}

				// updating items data packet, without it, client would get cursed textures inside world because it haven't gotten the items data correctly
				GameUpdatePacket *pRefreshItemsPacket = GetItemInfoManager()->GetUpdatePacket();
				if (pRefreshItemsPacket == NULL)
				{
					// the packet was null(fail), so we cannot really procceed updating data...
					pClient->SendLog("Something went wrong trying to update the items data.");
					LogError("ItemInfoManager >> Error! Seems like refreshing packet is null, check it out ASAP!");
					return;
				}

				// sending the packet to update the items
				pClient->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pRefreshItemsPacket, GUP_SIZE + pRefreshItemsPacket->dataLength, ENET_PACKET_FLAG_RELIABLE);
				return;
			}

			if (text_data.starts_with("action|enter_game"))
			{
				GrowPacketsListener::OnHandleGameEnter(pClient);
				return;
			}

			if (text_data.starts_with("action|helpmenu"))
			{
				GrowPacketsListener::OnHandleGameHelper(pClient, text_data.c_str());
				return;
			}

			break;
		}

		case NET_MESSAGE_GAME_MESSAGE:
		{
			// the genertic text message type is a text packet the client sends
			// common action text packets related to worlds, sent from the client use this msg type
			
			// actions using this message type:
			// action|quit
			// action|world_button
			// action|validate_world
			// action|join_request
			// action|quit_to_exit
			// action|gohomeworld
			if (pConnectionPeer->data == NULL || pPacket->dataLength > 1024)
			{
				// game client may be null
				// packet may exceed the lengths we support

				// in this case we cannot procceed handling the incoming packet
				LogError("failed to proccess incoming NET_MESSAGE_GAME_MESSAGE packet, something went wrong!(2)");
				return;
			}

			memset(pPacket->data + pPacket->dataLength - 1, 0, 1);
			// this is the text packet data the client sends
			const nova_str& text_data = (const char*)pPacket->data + 4;
			GameClient* pClient = (GameClient*)pConnectionPeer->data;
			if (pClient == NULL)
			{
				// game client is null
				LogError("failed to proccess incoming NET_MESSAGE_GAME_MESSAGE packet, client is NULL!");
				return;
			}

			printf("%s\n", text_data.c_str());
			if (text_data == "action|quit")
			{
				// client disconnects from the server instantly
				enet_peer_disconnect_later(pConnectionPeer, 0U);
				return;
			}

			if (text_data.starts_with("action|world_button"))
			{
				// world categories in main menu
				// in 3.xx, ubisoft changed the menu, fortunarely for us, we will handle both of them based on client's version :)
			}

			if (text_data.starts_with("action|join_request"))
			{
				GrowPacketsListener::OnHandleJoinRequest(pClient, text_data.c_str());
			}

			if (text_data.starts_with("action|quit_to_exit"))
			{
				GrowPacketsListener::OnHandleExit(pClient);
			}

			break;
		}

		case NET_MESSAGE_GAME_PACKET:
		{
			if (pConnectionPeer->data == NULL || pPacket->data == NULL || pPacket->dataLength < 60 || pPacket->dataLength > 61)
			{
				// game client may be null
				// packet data may be null
				// packet may not have the lengths we support
				// in this case we cannot procceed handling the incoming packet
				
				LogError("failed to proccess incoming NET_MESSAGE_GAME_PACKET packet, size mismatch.");
				return;
			}

			GameClient* pClient = (GameClient*)pConnectionPeer->data;
			if (pClient == NULL)
			{
				// game client is null
				LogError("failed to proccess incoming NET_MESSAGE_GAME_PACKET packet, client is NULL!");
				return;
			}

			// this message type is more complex, it contains it's own structure, original variables's name is unknown without access to the source code or reverse engineering it from the client
			// we managed to retrieve some of them, others are named after bruteforcing them manually
			// the structure has default size of 56 - sizeof(GameUpdatePacket)
			// there is cases where it has larger size, if in the flags column of it, has the flag NET_GAME_PACKET_FLAG_EXTENDED
			// the flag mentioned above allows the client to read tankpacket's data column, whereas dataLength column contains the length of the "extended data" written
			GameUpdatePacket* pTankPacket = reinterpret_cast<GameUpdatePacket*>(pPacket->data + 4);
			if (pTankPacket == NULL)
			{
				// tank packet is null
				LogError("failed to proccess incoming NET_MESSAGE_GAME_PACKET packet, tank packet is NULL!");
				return;
			}

			// tank packets are handled by their type, they are explained below
			switch (pTankPacket->type)
			{
				case NET_GAME_PACKET_STATE:
				{
			        TankPacketsListener::OnHandleStatePacket(pClient, pTankPacket);
					return;
			    }

				case NET_GAME_PACKET_UPDATE_STATUS:
				{
					World * pWorld = pClient->GetWorld();
					if (pWorld == NULL)
					{
						// world was null
						return;
					}

					// this packet type just needs it's netID modified, broadcasted, it shows the "chatting bubble", "BRB" bubble above the character when received
					pTankPacket->netID = pClient->GetNetID();

					pWorld->Broadcast([&](GameClient * pPlayer) {
						pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pTankPacket, sizeof(GameUpdatePacket) + pTankPacket->dataLength, ENET_PACKET_FLAG_RELIABLE);
					});
					break;
				}

				case NET_GAME_PACKET_TILE_CHANGE_REQUEST:
				{
					World * pWorld = pClient->GetWorld();
					if (pWorld == NULL)
					{
						// world was null
						return;
					}

					ItemInfo * pItemInfo = GetItemInfoManager()->GetItemByID(pTankPacket->intData);
					if (pItemInfo == NULL)
					{
						// item info was not found
						return;
					}

					switch (pItemInfo->type)
					{
					    case TYPE_FIST:
						    pWorld->HandlePacketTileChangeRequestPunch(pClient, pTankPacket);
						    break;

					    case TYPE_WRENCH:
						    pWorld->HandlePacketTileChangeRequestWrench(pClient, pTankPacket);
						    break;

					    default:
						    pWorld->HandlePacketTileChangeRequestPlace(pClient, pTankPacket, pItemInfo);
						    break;
					}

					break;
				}
			}

			break;
		}
	}
}