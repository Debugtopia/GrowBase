#ifndef QUITTOEXITLISTENER_H
#define QUITTOEXITLISTENER_H
#include <string>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

namespace GrowPacketsListener
{
	// sends the client the world selection menu & gazette
	void OnHandleExit(GameClient* pClient)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		World * pWorld = pClient->GetWorld();
		if (pWorld == NULL)
		{
			// is not in world
			VariantSender::OnConsoleMessage(pClient, "Cheating is prohibited!");
			return;
		}

		GetWorldsManager()->Exit(pClient);
	}

} // namespace GrowPacketsListener

#endif QUITTOEXITLISTENER_H