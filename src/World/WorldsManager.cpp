#include <BaseApp.h> // precompiled
#include <World/WorldsManager.h>

#include <Client/GameClient.h>
#include <World/World.h>

#include <SDK/Proton/MiscUtils.h>

WorldsManager g_worlds;
WorldsManager* GetWorldsManager() { return &g_worlds; }

WorldsManager::~WorldsManager()
{
	//
}

World * WorldsManager::GetWorldByName(const std::string& fName)
{
	bool bFound = false;
	for (int i = 0; i < m_activeWorlds.size(); i++)
	{
		World * pWorld = m_activeWorlds[i];
		if (pWorld == NULL)
		{
			// world is null
			continue;
		}

		nova_str upper_name = Utils::StringUppercase(fName);
		if (pWorld->GetName() != upper_name)
		{
			// name mismatching
			continue;
		}

		bFound = true;
		return pWorld;
	}

	if (!bFound)
	{
		// get from db
		return NULL;
	}

	return NULL;
}

World * WorldsManager::GetWorldByID(const int& ID)
{
	bool bFound = false;
	for (int i = 0; i < m_activeWorlds.size(); i++)
	{
		World* pWorld = m_activeWorlds[i];
		if (pWorld == NULL || pWorld->GetID() != ID)
		{
			// world is null or ID mismatch
			continue;
		}

		bFound = true;
		return pWorld;
	}

	if (!bFound)
	{
		// get from db
		return NULL;
	}

	return NULL;
}

void WorldsManager::SendWorldOffers(GameClient* pClient, const bool& bOnlineMessage)
{
	if (pClient == NULL)
	{
		// game client is null
		return;
	}

	// growtopia changed the menu in PAW Day 2 - 24th August, 2021(V3.68) so im gonna hardcode for old clients to not suffer from the retarded UI
	bool bUsingOldMenu = pClient->GetLoginDetails()->gameVersion < 3.69f;

	// lets build the menu now
	WorldOffersMenu menu;
	menu.SetDefault("TUTORIAL");
	if (bUsingOldMenu)
	{
		menu.AddButton("Showing: Random Worlds", "_-catselect-_", 0.7, -765805825);
	}
	else
	{
		menu.SetupSimpleMenu();
		menu.AddHeading("Top Worlds<ROW2>");
		menu.AddHeading("My Worlds<CR>");
		menu.AddHeading("Recently Visited Worlds<CR>");
	}

	if (bOnlineMessage)
	{
		VariantSender::OnConsoleMessage(pClient, "Where would you like to go? (`w1`` others online)");
	}

	VariantSender::OnRequestWorldSelectMenu(pClient, menu.Build());
}

bool WorldsManager::Enter(GameClient * pClient, const char * fName, CL_Vec2f spawnPoint)
{
	if (pClient == NULL)
	{
		// game client is null
		return false;
	}

	nova_str upper_name = Utils::StringUppercase(fName);
	World *  pWorld = GetWorldByName(upper_name);
	if (pWorld == NULL)
	{
		// creating a new world object
		pWorld = new World(upper_name);
		pWorld->GetWorldTileMap()->GenerateTerrain(TERRATYPE_SUNNY, 100, 60);
		m_activeWorlds.emplace_back(pWorld);
	}

	if (pWorld->HasBit(WORLDBIT_NOGO)) // missing moderator check
	{
		pClient->SendEntryFail("`4To reduce confusion, that is not a valid world name.`` Try another?");
		return false;
	}

	if (pWorld->HasBit(WORLDBIT_INACCESSIBLE)) // missing moderator check
	{
		pClient->SendEntryFail("That world is inaccessible.");
		return false;
	}

	// serializing world data and sending packet to the game client
	int                memOffset = 0;
	size_t             world_data_length = pWorld->GetMemoryEstimated(true, pClient->GetLoginDetails()->gameVersion, pWorld->GetMapVersion()); // getting the size of the world
	GameUpdatePacket * pMapDataPacket = (GameUpdatePacket*)std::malloc(sizeof(GameUpdatePacket) + world_data_length);
	if (pMapDataPacket == NULL)
	{
		// failed to allocate for the packet, missing resources maybe???
		pClient->SendEntryFail("Something failed while entering world.");
		nova_dealloc(pMapDataPacket);
		return false;
	}

	pMapDataPacket->type = NET_GAME_PACKET_SEND_MAP_DATA;
	pMapDataPacket->netID = -1;
	pMapDataPacket->flags |= NET_GAME_PACKET_FLAG_EXTENDED;
	pMapDataPacket->dataLength = (uint32_t)world_data_length;

	// copying world data to the packet & sending it to client and freeing it
	pWorld->Serialize(pMapDataPacket->data, memOffset, true, pClient->GetLoginDetails()->gameVersion, pWorld->GetMapVersion());
	pClient->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pMapDataPacket, sizeof(GameUpdatePacket) + pMapDataPacket->dataLength, ENET_PACKET_FLAG_RELIABLE);
	nova_dealloc(pMapDataPacket);

	if (spawnPoint == CL_Vec2f(0.f, 0.f))
	{
		// 0.f, 0.f is default / undefined position, if it's that case, we get the spawn point(main door's position) from the tile map
		spawnPoint = pWorld->GetWorldTileMap()->GetSpawnPoint();
	}

	pClient->SetWorld(pWorld);
	pClient->SetNetID(pWorld->GetNetID(true));
	pClient->SetPosition(spawnPoint.X, spawnPoint.Y);
	pClient->SetRespawnPos(spawnPoint.X, spawnPoint.Y);
	VariantSender::OnSpawn(pClient, pClient->GetSpawnData(true));
	pClient->SendVariantPacket({ "OnSetCurrentWeather", pWorld->GetActiveWeather() });
	pClient->SendVariantPacket({ "OnFlagMay2019", 256 }, pClient->GetNetID());
	pClient->SendVariantPacket({ "OnSetPos", spawnPoint }, pClient->GetNetID());
	pClient->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");
	pClient->SendVariantPacket({ "OnNameChanged", "`w" + pClient->GetDisplayName() + "``", pClient->GetTitleIcon() }, pClient->GetNetID());
	pClient->SendVariantPacket({ "OnCountryState", pClient->GetCountryState() }, pClient->GetNetID());
	pClient->SendChracterState(pClient);
	pClient->UpdateClothes(pClient);

	nova_str extra = "";
	if (pWorld->GetCategory() != WORLD_CATEGORY_NONE)
	{
		extra = " `w[`9" + pWorld->GetCategoryAsString() + "``]``";
	}

	//`0[```2NOPUNCH``, `2IMMUNE``, `4JAMMED``, `2NOWAR``, `2ANTIGRAVITY```0]`` ``
	VariantSender::OnConsoleMessage(pClient, "World `w" + pWorld->GetName() + "" + extra + "``. entered. There are `w" + std::to_string(pWorld->GetPlayersCount() - 1) + "`` other people here, `w0`` online.");
	if (pWorld->GetWorldLockIndex() == 0)
	{
		VariantSender::OnConsoleMessage(pClient, "Lock this world with world lock.");
	}
	else
	{
		// get lock owner name from cache & display the message
	}

	// broadcasting packets for others to see your character and you their
	for (int i = 0; i < pWorld->GetClients().size(); i++)
	{
		GameClient * pPlayer = pWorld->GetClients().at(i);
		if (pPlayer == NULL)
		{
			// player was null
			continue;
		}

		bool bSelf = pPlayer == pClient; // target player is the client we send to the world itself
		if (pPlayer->GetNetID() == pClient->GetNetID())
		{
			// target is himself
			continue;
		}

		VariantSender::OnSpawn(pClient, pPlayer->GetSpawnData(), -1, -1);
		VariantSender::OnSpawn(pPlayer, pClient->GetSpawnData(), -1, -1);

		pClient->SendVariantPacket({ "OnFlagMay2019", 256 }, pPlayer->GetNetID());
		pPlayer->SendVariantPacket({ "OnFlagMay2019", 256 }, pClient->GetNetID());

		if (true /* check if pClient has invisible flag on */)
		{
			pClient->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");
		    pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");
		}

		pClient->SendVariantPacket({ "OnNameChanged", "`w" + pPlayer->GetDisplayName() + "``", pPlayer->GetTitleIcon() }, pPlayer->GetNetID());
		pPlayer->SendVariantPacket({ "OnNameChanged", "`w" + pClient->GetDisplayName() + "``", pClient->GetTitleIcon() }, pClient->GetNetID());

		pClient->SendVariantPacket({ "OnCountryState", pPlayer->GetCountryState() }, pPlayer->GetNetID());
		pPlayer->SendVariantPacket({ "OnCountryState", pClient->GetCountryState() }, pClient->GetNetID());

		if (!bSelf && true /* check if pClient has invisible flag on */)
		{
			VariantSender::OnTalkBubble(pPlayer, pClient->GetNetID(), "`5<`w" + pClient->GetDisplayName() + "`` entered, `w" + std::to_string(pWorld->GetPlayersCount() - 1) + "`` others here>``");
            VariantSender::OnConsoleMessage(pPlayer, "`5<`w" + pClient->GetDisplayName() + "`` entered, `w" + std::to_string(pWorld->GetPlayersCount() - 1) + "`` others here>``");
			pPlayer->SendPacket(NET_MESSAGE_GAME_MESSAGE, "action|play_sfx\nfile|audio/door_open.wav\ndelayMS|0");

			if (true /* check if pPlayer has invisible flag on */)
			{
				VariantSender::OnTalkBubble(pClient, pPlayer->GetNetID(), pPlayer->GetDisplayName());
			}
		}

		pClient->SendChracterState(pPlayer);
		pPlayer->SendChracterState(pClient);

		pClient->UpdateClothes(pPlayer);
		pPlayer->UpdateClothes(pClient);
	}

	return true;
}