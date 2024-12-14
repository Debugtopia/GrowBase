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
		pClient->SendLog("Where would you like to go? (`w%d`` others online)", 1);
	}

	VariantList var;
	var.Get(0).Set("OnRequestWorldSelectMenu");
	var.Get(1).Set(menu.Build());
	pClient->SendVariantPacket(var);
}