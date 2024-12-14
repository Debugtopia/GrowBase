#ifndef LOGONPACKETLISTENER_H
#define LOGONPACKETLISTENER_H
#include <string>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

namespace GrowPacketsListener
{
	// guest / non-registered account logon handler
	// > used for accounts with names such as Buddy_123
	void OnHandleGuestLogon(GameClient* pClient, const nova_str& textPacket)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(textPacket.c_str());
		pClient->GetLoginDetails()->platformID = t.GetParmString("platformID", 1);
		pClient->GetLoginDetails()->requestedName = t.GetParmString("requestedName", 1);
		pClient->GetLoginDetails()->country = t.GetParmString("country", 1);
		pClient->GetLoginDetails()->gameVersion = t.GetParmFloat("game_version", 1);
		pClient->GetLoginDetails()->rid = t.GetParmString("rid", 1);
		pClient->GetLoginDetails()->mac = t.GetParmString("mac", 1);
		pClient->GetLoginDetails()->logonMode = t.GetParmInt("lmode", 1);
		pClient->GetLoginDetails()->doorID = t.GetParmString("doorID", 1);

		if (pClient->GetLoginDetails()->logonMode != (int)eLogonMode::LOGONMODE_JOINREQUEST)
		{
			pClient->SendVariantPacket({ "OnConsoleMessage", "One moment, updating items data..." });
		}

		nova_str urlPrefix = pClient->GetLoginDetails()->gameVersion >= 3.91f ? "www." : "";
		pClient->SendVariantPacket({ Utils::GetLogonVariantString(pClient->GetLoginDetails()->gameVersion),
			GetItemInfoManager()->GetHash(),
			urlPrefix + GetConfig().downloadServerURL,
			GetConfig().downloadServerPath,
			"cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster",
			"proto=209|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|clash_active=0|drop_lavacheck_faster=1|isPayingUser=1|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|"
		});

		t.Kill();
	}

	// growid / egistered account logon handler
	// > used for registered accounts
	void OnHandleGrowIDLogon(GameClient* pClient, const nova_str& textPacket)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(textPacket.c_str());
		pClient->GetLoginDetails()->platformID = t.GetParmString("platformID", 1);
		pClient->GetLoginDetails()->requestedName = t.GetParmString("requestedName", 1);
		pClient->GetLoginDetails()->tankIDName = t.GetParmString("tankIDName", 1);
		pClient->GetLoginDetails()->country = t.GetParmString("country", 1);
		pClient->GetLoginDetails()->gameVersion = t.GetParmFloat("game_version", 1);
		pClient->GetLoginDetails()->rid = t.GetParmString("rid", 1);
		pClient->GetLoginDetails()->mac = t.GetParmString("mac", 1);

		nova_str tankIDPass = t.GetParmString("tankIDPass", 1);
		// hash password here
		
		pClient->GetLoginDetails()->logonMode = t.GetParmInt("lmode", 1);
		pClient->GetLoginDetails()->doorID = t.GetParmString("doorID", 1);

		if (pClient->GetLoginDetails()->logonMode != (int)eLogonMode::LOGONMODE_JOINREQUEST)
		{
			pClient->SendVariantPacket({ "OnConsoleMessage", "One moment, updating items data..." });
		}

		nova_str urlPrefix = pClient->GetLoginDetails()->gameVersion >= 3.91f ? "www." : "";
		pClient->SendVariantPacket({ Utils::GetLogonVariantString(pClient->GetLoginDetails()->gameVersion),
			GetItemInfoManager()->GetHash(),
			urlPrefix + GetConfig().downloadServerURL,
			GetConfig().downloadServerPath,
			"cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster",
			"proto=209|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|clash_active=0|drop_lavacheck_faster=1|isPayingUser=1|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|"
		});

		t.Kill();
	}

	// legacy users / egistered account logon handler
	// > used for registered accounts using 4.61+ client game version
	void OnHandleTokenLogon(GameClient* pClient, const nova_str& textPacket)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(textPacket.c_str());

		nova_str login_token = t.GetParmString("ltoken", 1);
		nova_str token_decypher = base64_decode(login_token);
		// now, we have the login info decyphered, we can procceed handling it
		LogMsg("[DEBUG]: login token: %s\n%s", login_token.c_str(), token_decypher.c_str());

		// handling as soon as it's figured out how it works
		nova_str urlPrefix = pClient->GetLoginDetails()->gameVersion >= 3.91f ? "www." : "";
		pClient->SendVariantPacket({ Utils::GetLogonVariantString(pClient->GetLoginDetails()->gameVersion),
			GetItemInfoManager()->GetHash(),
			urlPrefix + GetConfig().downloadServerURL,
			GetConfig().downloadServerPath,
			"cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster",
			"proto=209|choosemusic=audio/mp3/about_theme.mp3|active_holiday=0|clash_active=0|drop_lavacheck_faster=1|isPayingUser=1|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|"
		});

		t.Kill();
	}


} // namespace GrowPacketsListener

#endif LOGONPACKETLISTENER_H