#ifndef ENTERGAMELISTENER_H
#define ENTERGAMELISTENER_H
#include <string>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

namespace GrowPacketsListener
{
	// sends the client the world selection menu & gazette
	void OnHandleGameEnter(GameClient* pClient)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		nova_str player_name = pClient->GetName();
		pClient->SendLog("Welcome back, `w%s``.", player_name.c_str());
		GetWorldsManager()->SendWorldOffers(pClient, true);
	}

} // namespace GrowPacketsListener

#endif ENTERGAMELISTENER_H