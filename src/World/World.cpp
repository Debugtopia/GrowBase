#include <BaseApp.h> // precompiled
#include <World/World.h>

#include <Client/GameClient.h>

World::World(const std::string& name, const uint8_t& width, const uint8_t& height)
{
	nova_delete(m_pWorldTileMap);
	nova_delete(m_pWorldObjectMap);

	m_name = name;
	m_pWorldTileMap = new WorldTileMap(width, height);
	m_pWorldObjectMap = new WorldObjectMap();
}

World::~World()
{
	nova_delete(m_pWorldTileMap);
	nova_delete(m_pWorldObjectMap);
}

void World::Broadcast(std::function<void(int, GameClient*)> fCall)
{
	// these were taken from beef - thanks kevz
	for (int i = 0; i < m_clients.size(); i++)
	{
		GameClient * pClient = m_clients[i];
		if (pClient == NULL)
		{
			// client was null
			continue;
		}

		fCall(pClient->GetNetID(), pClient);
	}
}

void World::Broadcast(std::function<void(GameClient*)> fCall)
{
	// these were taken from beef - thanks kevz
	for (int i = 0; i < m_clients.size(); i++)
	{
		GameClient * pClient = m_clients[i];
		if (pClient == NULL)
		{
			// client was null
			continue;
		}

		fCall(pClient);
	}
}

bool World::HasBit(const int& bit)
{
	return m_bits & bit;
}

nova_str World::GetCategoryAsString()
{
	switch (m_category)
	{
	    case WORLD_CATEGORY_ADVENTURE:
			return "Adventure";
		case WORLD_CATEGORY_ART:
			return "Art";
		case WORLD_CATEGORY_FARM:
			return "Farm";
		case WORLD_CATEGORY_GAME:
			return "Game";
		case WORLD_CATEGORY_GUILD:
			return "Guild";
		case WORLD_CATEGORY_INFORMATION:
			return "Information";
		case WORLD_CATEGORY_MUSIC:
			return "Music";
		case WORLD_CATEGORY_PARKOUR:
			return "Parkour";
		case WORLD_CATEGORY_PUZZLE:
			return "Puzzle";
		case WORLD_CATEGORY_ROLEPLAY:
			return "Roleplay";
		case WORLD_CATEGORY_SHOP:
			return "Shop";
		case WORLD_CATEGORY_SOCIAL:
			return "Social";
		case WORLD_CATEGORY_STORAGE:
			return "Storage";
		case WORLD_CATEGORY_STORY:
			return "Story";
		case WORLD_CATEGORY_TRADE:
			return "Trade";
	}

	return "None";
}

int World::GetPlayersCount()
{
	int count = 0;
	for (int i = 0; i < m_clients.size(); i++)
	{
		GameClient * pClient = m_clients[i];
		if (pClient == NULL)
		{
			// client was null
			continue;
		}

		// check if invis then continue

		++count;
	}

	return count;
}

size_t World::GetMemoryEstimated(const bool& bClientSide, const float& fClientVersion, const uint16_t& worldMapVersion)
{
	if (m_pWorldTileMap == NULL || m_pWorldObjectMap == NULL)
	{
		// tile or object map is null
		return 0;
	}

	size_t estimated = 0;
	estimated += sizeof(uint16_t); // map version
	estimated += sizeof(int); // bits
	estimated += sizeof(uint16_t) + m_name.length();

	estimated += m_pWorldTileMap->GetMemoryEstimated(bClientSide, fClientVersion, worldMapVersion);
	if (bClientSide && fClientVersion >= 4.31f)
	{
		// some new ubisoft garbage for client side only
		estimated += sizeof(int);
		estimated += sizeof(int);
		estimated += sizeof(int);
	}

	estimated += m_pWorldObjectMap->GetMemoryEstimated();
	estimated += sizeof(int) * 2; // active & base weather machine ID
	return estimated;
}

void World::ToggleBit(const int& bit, const bool& bSetAsActive)
{
	if (HasBit(bit) && bSetAsActive == false)
	{
		// removing bit
		m_bits &= ~bit;
	}

	if (HasBit(bit) == false && bSetAsActive)
	{
		// adding bit
		m_bits |= bit;
	}
}

void World::Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide, const float& fClientVersion, const uint16_t& worldMapVersion)
{
    if (pData == NULL || m_pWorldTileMap == NULL || m_pWorldObjectMap == NULL)
	{
        // data, tile map or object map is null
		return;
	}

	MemorySerializeRaw(m_mapVersion, pData, memOffset, true);
	MemorySerializeRaw(m_bits, pData, memOffset, true);
	MemorySerialize(m_name, pData, memOffset, true);
	m_pWorldTileMap->Serialize(pData, memOffset, bClientSide, fClientVersion, worldMapVersion);
	
	if (bClientSide && fClientVersion >= 4.31f)
	{
		// some new ubisoft garbage for client side only
		int zero = 0;
		MemorySerializeRaw(zero, pData, memOffset, true);
		MemorySerializeRaw(zero, pData, memOffset, true);
		MemorySerializeRaw(zero, pData, memOffset, true);
	}

	m_pWorldObjectMap->Serialize(pData, memOffset, bClientSide);
	MemorySerializeRaw(m_activeWeather, pData, memOffset, true);
	MemorySerializeRaw(m_baseWeather, pData, memOffset, true);
}

void World::HandlePacketTileChangeRequestPunch(GameClient * pClient, GameUpdatePacket * pPacket)
{
	if (pClient == NULL || pPacket == NULL)
	{
		// client or tank packet was null
		return;
	}

	if (m_pWorldTileMap == NULL)
	{
		// tile map was null
		return;
	}

	CL_Vec2f interactedAt = { (float)pPacket->intX, (float)pPacket->intY };
	Tile * pTile = m_pWorldTileMap->GetTile(interactedAt);
	if (pTile == NULL)
	{
	    // tile was not found
		return;
	}

	ItemInfo * pItemInfo = pTile->GetItemInfo();
	if (pItemInfo == NULL)
	{
		// item info was not found
		return;
	}

	if (pItemInfo->ID == 0)
	{
		// tile does not exist - what did you expect to punch, lmao...
		return;
	}

	if (nova_clock::now() - pTile->DamageTick >= std::chrono::seconds(pItemInfo->regenTime))
	{
		// reset the tile's damage
		pTile->SetDamage(0);
		pTile->DamageTick = nova_clock::now();
	}

	if (pItemInfo->editableTypes & MOD)
	{
		std::string msg = "It's too strong to break.";
		if (pItemInfo->type == TYPE_MAIN_DOOR)
		{
			msg = "(stand over and punch to use)";
		}

		VariantSender::OnTalkBubble(pClient, pClient->GetNetID(), msg, 0, true);
		pClient->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
		return;
	}

	switch (pItemInfo->type)
	{
	    case TYPE_SWITCHEROO: case TYPE_SWITCHEROO2: case TYPE_BOOMBOX: case TYPE_BACK_BOOMBOX: case TYPE_CHEST: case TYPE_GREEN_FOUNTAIN: case TYPE_DEADLY_IF_ON: case TYPE_LIGHTNING_IF_ON: case TYPE_LAB: case TYPE_STEAMPUNK:
		{
			if (pItemInfo->ID == ITEM_ID_ANGRY_ADVENTURE_GORILLA)
			{
				break;
			}

			pTile->ToggleFlag(TILEFLAG_ENABLED, !pTile->HasFlag(TILEFLAG_ENABLED));
			switch (pItemInfo->ID)
			{
				case ITEM_ID_SIGNAL_JAMMER:
				{
					ToggleBit(WORLDBIT_JAMMED, pTile->HasFlag(TILEFLAG_ENABLED));
					nova_str status = pTile->HasFlag(TILEFLAG_ENABLED) ? "`4hidden``" : "`2visible``";
					
					Broadcast([&](GameClient * pPlayer) {
						pPlayer->SendVariantPacket({ "OnConsoleMessage", "Signal Jammer enabled. This world is now " + status + " from the universe."});
					});
					break;
				}

				case ITEM_ID_ZOMBIE_JAMMER:
				{
					ToggleBit(WORLDBIT_IMMUNE, pTile->HasFlag(TILEFLAG_ENABLED));
					break;
				}

				case ITEM_ID_PUNCH_JAMMER:
				{
					ToggleBit(WORLDBIT_NOPUNCH, pTile->HasFlag(TILEFLAG_ENABLED));
					break;
				}

				case ITEM_ID_BALLOON_JAMMER:
				{
					ToggleBit(WORLDBIT_NOWAR, pTile->HasFlag(TILEFLAG_ENABLED));
					break;
				}

				case ITEM_ID_ANTIGRAVITY_GENERATOR:
				{
					ToggleBit(WORLDBIT_ANTIGRAVITY, pTile->HasFlag(TILEFLAG_ENABLED));
					break;
				}

				case ITEM_ID_GUARDIAN_PINEAPPLE:
				{
					if (pTile->GetDamage() == 0)
					{
						ToggleBit(WORLDBIT_NOCONSUME, pTile->HasFlag(TILEFLAG_ENABLED));
					}
					else
					{
						VariantSender::OnTalkBubble(pClient, pClient->GetNetID(), "Guardian Pineapple cannot be used while it's damaged.", 0, true);
						pClient->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
						return;
					}

					break;
				}

				case ITEM_ID_MINI_MOD:
				{
					ToggleBit(WORLDBIT_NODROP, pTile->HasFlag(TILEFLAG_ENABLED));
					break;
				}
			}

			break;
	    }
	}

	pPacket->type = NET_GAME_PACKET_TILE_APPLY_DAMAGE;
	pPacket->netID = pClient->GetNetID();
	pPacket->tileDamage = pClient->GetHitPower();

	pTile->SetDamage(pTile->GetDamage() + pClient->GetHitPower());
	pTile->DamageTick = nova_clock::now();

	// checking if tile was broken or not
	if (pTile->GetDamage() >= pItemInfo->hardness) 
	{
		pPacket->type = NET_GAME_PACKET_TILE_CHANGE_REQUEST;
		pPacket->intData = ITEM_ID_FIST;

		/*if (pItemInfo->editableTypes & AUTOPICKUP && pClient->GetInventoryItemCount(pItemInfo->ID) + 1 > pItemInfo->maxCount)
		{
			pClient->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
			VariantSender::OnTalkBubble(pClient, pClient->GetNetID(), "I better not break that, I have no room to pick it up!", 0, true);
			return;
		}*/

		switch (pItemInfo->type) 
		{
		    case TYPE_CHECKPOINT:
		    {
			    for (int i = 0; i < m_clients.size(); i++)
			    {
				    GameClient * pClient = m_clients[i];
				    if (pClient == NULL)
				    {
					    // client was null
					    continue;
				    }

				    if (pClient->GetRespawnPos().X / 32.f != pPacket->intX || pClient->GetRespawnPos().Y / 32.f != pPacket->intY)
					{
						continue;
				    }

					pClient->SetRespawnPos(m_pWorldTileMap->GetSpawnPoint());
					pClient->SendVariantPacket({ "SetRespawnPos", m_pWorldTileMap->GetSpawnPoint().X / 32.f + (m_pWorldTileMap->GetSpawnPoint().Y / 32.f * m_pWorldTileMap->GetWidth()) }, pClient->GetNetID());
			    }

				break;
		    }
		}

		// handle goodie

		// handle xp

		if (!(pItemInfo->editableTypes & AUTOPICKUP) && pItemInfo->rarity != 999)
		{
			bool  bLucky = false;
			bool  bBlock = false;
			bool  bSeed = false;
			int   gems = 0;

			float spawnX = pPacket->intX * 32.f;
			float spawnY = pPacket->intY * 32.f;

			if (bBlock)
			{
				WorldObject obj;
				obj.itemID = pItemInfo->ID;
				obj.x = spawnX;
				obj.y = spawnY;
				obj.count = 1;

				//TODO: other buffs & add
			}

			if (bSeed)
			{
				WorldObject obj;
				obj.itemID = pItemInfo->ID + 1;
				obj.x = spawnX;
				obj.y = spawnY;
				obj.count = 1;

				//TODO: other buffs & add
			}

			if (gems > 0) 
			{
				if (bLucky) 
				{
					if (Randomizer::Get(0, 100) < 98)
					{
						// removing extra gems
						gems = gems / 5;
					}
					else
					{
						Broadcast([&](GameClient * pPlayer) { 
							pPlayer->SendVariantPacket({ "OnParticleEffect", 125, CL_Vec2f(spawnX, spawnY), 0.0f, 0.0f }); 
						});
					}
				}

				// drop gems
			}
		}

		pTile->SetDamage(0);
		pTile->ResetTileExtra();
		pTile->ResetNeccesaryFlags();
		if (pItemInfo->type == TYPE_BACKGROUND)
		{
			pTile->SetBackground(ITEM_ID_BLANK);
		}
		else
		{
			pTile->SetForeground(ITEM_ID_BLANK);
		}
	}

	Broadcast([&](GameClient * pPlayer) {
		pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength);
	});
}

void World::HandlePacketTileChangeRequestPlace(GameClient * pClient, GameUpdatePacket * pPacket, ItemInfo * pItemInfo)
{
	if (pClient == NULL || pPacket == NULL || pItemInfo == NULL)
	{
		// client, tank packet or item info was null
		return;
	}

	if (m_pWorldTileMap == NULL)
	{
		// tile map was null
		return;
	}

	Broadcast([&](GameClient * pPlayer) {
		pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength, ENET_PACKET_FLAG_RELIABLE);
	});
}

void World::HandlePacketTileChangeRequestConsume(GameClient * pClient, GameUpdatePacket * pPacket, ItemInfo * pItemInfo)
{
	if (pClient == NULL || pPacket == NULL || pItemInfo == NULL)
	{
		// client, tank packet or item info was null
		return;
	}

	if (m_pWorldTileMap == NULL)
	{
		// tile map was null
		return;
	}

	Broadcast([&](GameClient * pPlayer) {
		pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pPacket, sizeof(GameUpdatePacket) + pPacket->dataLength, ENET_PACKET_FLAG_RELIABLE);
	});
}

void World::HandlePacketTileChangeRequestWrench(GameClient * pClient, GameUpdatePacket * pPacket)
{
	if (pClient == NULL || pPacket == NULL)
	{
		// client or tank packet was null
		return;
	}

	if (m_pWorldTileMap == NULL)
	{
		// tile map was null
		return;
	}
}