#include <BaseApp.h> // precompiled

#include <Client/GameClient.h>
#include <World/World.h>

GameClient::GameClient(ENetPeer * pConnectionPeer)
{
	/* storing player as peer's data, it's deleted upon disconnect */
	pConnectionPeer->data = this;
	m_pConnectionPeer = pConnectionPeer;

	LoginDetails * pLoginDetails = &m_loginDetails;
	enet_address_get_host_ip(&pConnectionPeer->address, pLoginDetails->address, 16);

	m_items = PlayerItems();
}

GameClient::~GameClient()
{
	//
}

World * GameClient::GetWorld()
{
	return m_pWorld;
}

nova_str GameClient::GetName()
{
	if (m_accountID == -1)
	{
		return m_loginDetails.tankIDName;
	}

	return m_loginDetails.requestedName + "_" + std::to_string(m_accountID);
}

nova_str GameClient::GetDisplayName()
{
	nova_str name = GetName();
	if (m_nameOverride.empty() == false)
	{
		// player is nicked
		name = m_nameOverride;
	}

	nova_str prefix = "`w";
	nova_str suffix = "";

	if (m_pWorld && m_pWorld->GetWorldLockIndex() != 0 && m_pWorld->GetWorldOwnerID() != -1)
	{
		Tile * pLockTile = m_pWorld->GetWorldTileMap()->GetTile(m_pWorld->GetWorldLockIndex());
		if (pLockTile == NULL)
		{
			// lock tile not found
			goto skipLockCrap;
		}

		// TODO: get extra data for lock in the world, set admin & owner name colors
	}

	if (m_nameOverride.empty() == false)
	{
		// nicked, so hiding the suffix
		suffix = "";
	}

skipLockCrap:;
    // TODO: get role full prefix and set as prefix if found

    return prefix + name + suffix + "``";
}

nova_str GameClient::GetStaticDisplayName()
{
	nova_str name = GetName();
	if (m_nameOverride.empty() == false)
	{
		// player is nicked
		name = m_nameOverride;
	}

	nova_str prefix = "`w";
	nova_str suffix = "";

	if (m_nameOverride.empty() == false)
	{
		// nicked, so hiding the suffix
		suffix = "";
	}

    // TODO: get role full prefix and set as prefix if found

    return prefix + name + suffix + "``";
}

nova_str GameClient::GetStaticName()
{
	nova_str name = GetName();
	nova_str prefix = "`w";
	nova_str suffix = "";

	if (m_nameOverride.empty() == false)
	{
		// nicked, so hiding the suffix
		suffix = "";
	}

	// TODO: get role full prefix and set as prefix if found

	return prefix + name + suffix + "``";
}

nova_str GameClient::GetCountryState()
{
	nova_str countryStateData = m_loginDetails.country;

	// TODO: set countryStateData to lg before extra flags if honor title flags are active
	// TODO: append |maxLevel if max level title flags are active
	// TODO: append |doctor if doctor title flags are active
	// TODO: append |donor if grow4good title flags are active
	// TODO: append |euphoriaTitle if party title flags are active

	return countryStateData;
}

nova_str GameClient::GetTitleIcon()
{
	nova_str icon = "||";

	// this has some complexity going on due to client version changes, gotta be hardcoded so we will handle it later on
	return icon;
}

std::string GameClient::GetSpawnData(const bool& bLocalPawn)
{
	int mod_state = 1;
	int super_mod_state = 1;

	// TODO: handle mod states when roles are handled

	std::string data = "spawn|avatar\n";
	data.append("netID|" + std::to_string(m_netID) + "\n");
	data.append("userID|" + std::to_string(m_userID) + "\n");
	data.append("colrect|0|0|20|30\n");
	data.append("posXY|" + std::to_string((int)m_vec.X) + "|" + std::to_string((int)m_vec.Y) + "\n");
	data.append("name|" + GetStaticName() + "\n");
	data.append("titleIcon|" + GetTitleIcon() + "\n");
	data.append("country|" + m_loginDetails.country + "\n");
	data.append("invis|0\n");
	data.append("mstate|" + std::to_string(mod_state) + "\nsmstate|" + std::to_string(super_mod_state) + "\n");
	data.append("onlineID|" + std::to_string(m_onlineID) + "");
	
	if (bLocalPawn)
	{
		// if bLocalPawn is true, it will make the new character move with your character(will look hella buggy and eventually not let u move at all)
		// only enable bLocalPawn when it's sent upon entering world(OnSpawn)
		// if you wish to create 'vanish' system, using this may be useful, but is complex to handle it well and not fuck up the net avatar on the client side
		data.append("\ntype|local");
	}

	return data;
}

unsigned int GameClient::GetSkinColor()
{
	return m_items.GetSkinColor();
}

float GameClient::GetGravity()
{
	float gravity = m_gravity;

	return gravity;
}

float GameClient::GetSpeed()
{
	float speed = m_speed;

	return speed;
}

bool GameClient::HasStateFlag(const int& flag)
{
	return m_characterStateFlags & flag;
}

bool GameClient::HasEffectFlag(const int& flag)
{
	return m_characterEffectFlags & flag;
}

size_t GameClient::GetInventoryMemoryEstimated()
{
	bool bUsingOldPacket = m_loginDetails.gameVersion < 3.02;
	size_t estimated = 0;

	estimated += sizeof(uint8_t); // inventory version
	estimated += sizeof(int); // backpack slots
	estimated += bUsingOldPacket ? sizeof(uint8_t) : sizeof(uint16_t); // items length

	for (int i = 0; i < m_items.GetItems().size(); i++)
	{
		estimated += sizeof(uint16_t); //itemID
		estimated += sizeof(uint8_t); //count
		estimated += sizeof(uint8_t); //flags
	}

	return estimated;
}

void GameClient::SetWorld(World * pWorld)
{
	m_pWorld = pWorld;
}

void GameClient::ToggleStateFlag(const int& bit, const bool& bSetAsActive)
{
	if (HasStateFlag(bit) && bSetAsActive == false)
	{
		// removing bit
		m_characterStateFlags &= ~bit;
	}

	if (HasStateFlag(bit) == false && bSetAsActive)
	{
		// adding bit
		m_characterStateFlags |= bit;
	}
}

void GameClient::ToggleEffectFlag(const int& bit, const bool& bSetAsActive)
{
	if (HasEffectFlag(bit) && bSetAsActive == false)
	{
		// removing bit
		m_characterEffectFlags &= ~bit;
	}

	if (HasEffectFlag(bit) == false && bSetAsActive)
	{
		// adding bit
		m_characterEffectFlags |= bit;
	}
}

void GameClient::SerializeInventoryData(uint8_t * pData, int& memOffset)
{
	if (pData == NULL)
	{
		// data was null
		return;
	}

	bool bUsingOldPacket = m_loginDetails.gameVersion < 3.02;
	u8   inventory_version = bUsingOldPacket ? 0 : 1;
	MemorySerializeRaw(inventory_version, pData, memOffset, true);

	int  slots = (int)GetItems()->GetBackpackSlots();
	MemorySerializeRaw(slots, pData, memOffset, true);
	
	// writing items length
	if (bUsingOldPacket)
	{
		u8 size = (u8)m_items.GetItems().size();
		MemorySerializeRaw(size, pData, memOffset, true);
	}
	else
	{
		u16 size = (u16)m_items.GetItems().size();
		MemorySerializeRaw(size, pData, memOffset, true);
	}

	// writing inventory items data
	for (int i = 0; i < m_items.GetItems().size(); i++)
	{
		PlayerInventoryItem it = m_items.GetItems().at(i);
		MemorySerializeRaw(it.itemID, pData, memOffset, true);
		MemorySerializeRaw(it.count, pData, memOffset, true);

		u8         flags = 0;
		ItemInfo * pItemInfo = GetItemInfoManager()->GetItemByID(it.itemID);
		if (pItemInfo && pItemInfo->type == TYPE_CLOTHES)
		{
			for (int i = 0; i < NUM_CLOTHES; i++)
			{
				if (m_items.GetCloth(i) == it.itemID)
				{
					// item is worn, so giving equip flag to it
					// this is not required, i just do it to prevent visual issues with clothes
					// you could remove it if u want
					flags = INVENTORY_ITEM_FLAG_EQUIPPED;
				}
			}
		}

		it.flags = flags;
		MemorySerializeRaw(it.flags, pData, memOffset, true);
	}
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

void GameClient::SendPacketRaw(eNetMessageType messageType, const void* pRawData, const uintmax_t& packetLen, const enet_uint32& packetFlags)
{
	// default value of packetFlags is ENET_PACKET_FLAG_RELIABLE
	if (m_pConnectionPeer == NULL || m_pConnectionPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// enet connection is null or it's state is not connected to our server, therefore we cannot send the packet
		return;
	}

	// this is the packet we send to the game client
	ENetPacket * pClientPacket = enet_packet_create(NULL, 5 + packetLen, packetFlags);
	int memoffset = 0;
	if (pClientPacket == NULL)
	{
		// failed to create packet, so it would crash if we procceed writing stuff into it
		return;
	}

	// eMsg stands for header of the game client packet - every packet has that header
	std::memcpy(pClientPacket->data, &messageType, 4);
	pClientPacket->data[packetLen + 4] = 0;
	if (pRawData != NULL)
	{
		// null check to prevent crash upon copying the packet data to the packet's data
		std::memcpy(pClientPacket->data + 4, pRawData, packetLen);
	}

	if (enet_peer_send(m_pConnectionPeer, 0, pClientPacket) != 0)
	{
		// destroying the packet incase it was not sent to the client
		enet_packet_destroy(pClientPacket);
	}
}

void GameClient::SendPacket(eNetMessageType messageType, const std::string& textData)
{
	if (m_pConnectionPeer == NULL || m_pConnectionPeer->state != ENET_PEER_STATE_CONNECTED)
	{
		// enet connection is null or it's state is not connected to our server, therefore we cannot send the packet
		return;
	}

	// sending a text packet to the client
	SendPacketRaw(messageType, textData.data(), textData.size(), ENET_PACKET_FLAG_RELIABLE);
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
	u8 * pVariantData = variant.SerializeToMem(&variantPacketLen);
	if (pVariantData == NULL)
	{
		// failed to serialize variant to mem
		nova_dealloc(pVariantData);
		return;
	}

	// this is the tank packet we send to the client
	GameUpdatePacket * pVariantListPacket = (GameUpdatePacket *)std::malloc(sizeof(GameUpdatePacket) + variantPacketLen);
	if (pVariantListPacket == NULL)
	{
		// failed to allocate for the packet, missing resources maybe???
		nova_dealloc(pVariantListPacket);
		return;
	}

	pVariantListPacket->type = NET_GAME_PACKET_CALL_FUNCTION;
	pVariantListPacket->netID = netID;
	pVariantListPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pVariantListPacket->delay = delayMS;
	pVariantListPacket->dataLength = (u32)variantPacketLen;
	nova_memcopy(&pVariantListPacket->data, pVariantData, variantPacketLen, offset);

	SendPacketRaw(NET_MESSAGE_GAME_PACKET, pVariantListPacket, sizeof(GameUpdatePacket) + pVariantListPacket->dataLength);
	// freeing the packet from memory when sent to client so it won't leak memory
	nova_dealloc(pVariantListPacket);
	nova_dealloc(pVariantData);
}

void GameClient::SendInventoryState()
{
	bool               bUsingOldPacket = m_loginDetails.gameVersion < 3.02;
	u32                dataLength = (u32)GetInventoryMemoryEstimated();
	int                memOffset = 0;
	GameUpdatePacket * pInventoryStatePacket = (GameUpdatePacket*)malloc(sizeof(GameUpdatePacket) + dataLength);
	if (pInventoryStatePacket == NULL)
	{
		// allocation failed, missing resources???
		nova_dealloc(pInventoryStatePacket);
		return;
	}

	pInventoryStatePacket->type = NET_GAME_PACKET_SEND_INVENTORY_STATE;
	pInventoryStatePacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pInventoryStatePacket->dataLength = dataLength;

	SerializeInventoryData(pInventoryStatePacket->data, memOffset); // serializing inventory data to packet
	SendPacketRaw(NET_MESSAGE_GAME_PACKET, pInventoryStatePacket, sizeof(GameUpdatePacket) + pInventoryStatePacket->dataLength, ENET_PACKET_FLAG_RELIABLE);
	nova_dealloc(pInventoryStatePacket);
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
	if (!errMsg.empty())
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

void GameClient::SendChracterState(GameClient * pClient)
{
	if (pClient == NULL)
	{
		// target client was null
		return;
	}

	// sending character update packet
	GameUpdatePacket state;
	state.type = NET_GAME_PACKET_SET_CHARACTER_STATE;
	state.punchID = 0;
	state.buildRange = 128;
	state.punchRange = 128;
	state.netID = m_netID;
	state.flags = m_characterEffectFlags;
	state.waterSpeed = GetWaterSpeed();
	state.effectFlags = m_characterStateFlags;
	state.accel = GetAcceleration();
	state.punchStrength = GetKnockPower();
	state.speedOut = GetSpeed();
	state.gravityOut = GetGravity();
	state.pupilColor = GetLenColor();
	state.eyeColor = GetEyesColor();
	state.hairColor = GetHairColor();

	pClient->SendPacketRaw(NET_MESSAGE_GAME_PACKET, &state, sizeof(GameUpdatePacket), ENET_PACKET_FLAG_RELIABLE);
}

void GameClient::UpdateClothes(GameClient * pClient, const bool& bEquipSound)
{
	if (pClient == NULL)
	{
		// target client was null
		return;
	}

	// updating clothes to the target player
	pClient->SendVariantPacket({ "OnSetClothing",
		CL_Vec3f((float)GetItems()->GetCloth(HAT), (float)GetItems()->GetCloth(SHIRT), (float)GetItems()->GetCloth(PANTS)),
		CL_Vec3f((float)GetItems()->GetCloth(SHOES), (float)GetItems()->GetCloth(FACEITEM), (float)GetItems()->GetCloth(HAND)),
		CL_Vec3f((float)GetItems()->GetCloth(BACK), (float)GetItems()->GetCloth(HAIR), (float)GetItems()->GetCloth(CHESTITEM)),
		GetSkinColor(),
		CL_Vec3f((float)GetItems()->GetCloth(ARTIFACT), (float)bEquipSound, 0.f /* what the fuck is this shit */)
	}, m_netID);
}