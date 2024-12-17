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

		GameDialog dialog;
		dialog.SetDefaultColor('o')
		->AddLabelWithIcon(eDialogElementSizes::BIG, "`wWelcome to GrowBase``", eDialogElementDirections::LEFT, ITEM_ID_NEWSPAPER)
		->AddSpacer(eDialogElementSizes::SMALL)
		->AddTextbox("GrowBase is an open sourced Growtopia Private Server project, providing with well-structured and developed entirely from scratch source code.")
		->AddSmallText("`5Official Developer: ``@xhexago on telegram & discord.")
		->AddURLButton("discord_server", "`wDiscord Server``", "https://discord.gg/z27evDGqhy", "Do you wish to join GrowBase's discord server?")
		->AddURLButton("source_code", "`wGithub Repository``", "https://github.com/debugtopia/GrowBase", "Do you wish to open the official Github repository for GrowBase's source code?")
		->EndDialog("gazette", "", "OK");

		pClient->GetFunc().OnDialogRequest(dialog.Build());
	}

} // namespace GrowPacketsListener

#endif ENTERGAMELISTENER_H