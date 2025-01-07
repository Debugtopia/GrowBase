#ifndef GAMEHELPERLISTENER_H
#define GAMEHELPERLISTENER_H
#include <string>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

namespace GrowPacketsListener
{
	// sends the client the world selection menu & gazette
	void OnHandleGameHelper(GameClient* pClient, const char* pChar)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		World* pWorld = pClient->GetWorld();
		if (pWorld == NULL)
		{
			// is not in world
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(pChar);

		const nova_str& location = t.GetParmString("location", 1);
		// TODO: maybe add some check for location here


		// btw, this action seems only to show up on windows builds for some reason, but we handle it anyways
		GameDialog dialog;
		dialog.SetDefaultColor('o')
			->AddLabelWithIcon(eDialogElementSizes::BIG, "`wHow To Play GrowBase``", eDialogElementDirections::LEFT, ITEM_ID_NEWSPAPER)
			->AddSpacer(eDialogElementSizes::SMALL)
			->AddTextbox("`wWhat is GrowBase?``")
			->AddSmallText("- GrowBase is an open sourced Growtopia Private Server project, providing with well-structured and developed entirely from scratch source code.")
			->AddSmallText("- The reason behind hosting of GrowBase is because we want players to enjoy the server, in deeper stage of development, hosted by the creators.")
			->AddSmallText("- `5Official Developer: ``@xhexago on telegram & discord.")
			->AddURLButton("source_code", "`wGithub Repository``", "https://github.com/debugtopia/GrowBase", "Do you wish to open the official Github repository for GrowBase's source code?")
			->AddSpacer(eDialogElementSizes::SMALL)
			->AddTextbox("`wHow to obtain items?``")
			->AddSmallText("- Similiar to real servers, you need to earn them by farming, splicing, combining or completing quests. The diffence is, in GrowBase, doing such things is easier.")
			->AddSmallText("`2* Pro Tip: ```#@Moderators`` and `6@Super Moderators`` often host a public/private giveaways, so try to keep up with them and test your luck to gain some nice goodies.")
			->AddURLButton("discord_server", "`wDiscord Server``", "https://discord.gg/novaps", "Do you wish to join GrowBase's discord server?")
			->AddSpacer(eDialogElementSizes::SMALL)
			->AddSmallText("`8* Hacker Tip: ``Try using /rewards to earn some `9extrrrraaaaaaa special`` items, some of them are `4unobtainable`` in other ways.")
			->AddSpacer(eDialogElementSizes::SMALL)
			->AddTextbox("`wHow to support the server?``")
			->AddSmallText("- We are enlighted to tell you that GrowBase accepts donations, in term of `9Crypto``, `3Paypal`` or `1Diamond Locks`` payments, the funds from the donations will be put in real-money giveaways in our discord server, or used to fund GrowBase's public hosting.")
			->AddSmallText("`4 * Refunds: ``Unortunately, there are no possible refunds on the donated funds, so do not ask for one!")
			->AddSpacer(eDialogElementSizes::SMALL)
			->AddSmallText("- made with <3 by the GrowBase development team.")
			->EndDialog("grow_helper", "", "OK");

		VariantSender::OnDialogRequest(pClient, dialog.Build());
	}

} // namespace GrowPacketsListener

#endif GAMEHELPERLISTENER_H