#ifndef LOGONPACKETLISTENER_H
#define LOGONPACKETLISTENER_H
#include <string>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

namespace GrowPacketsListener
{
	// guest / non-registered account logon handler
	// > used for accounts with names such as Buddy_123
	void OnHandleGuestLogon(GameClient* pClient, const std::string& textPacket)
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
		t.Kill();
	}

	// growid / egistered account logon handler
	// > used for registered accounts
	void OnHandleGrowIDLogon(GameClient* pClient, const std::string& textPacket)
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

		std::string tankIDPass = t.GetParmString("tankIDPass", 1);
		// hash password here

		t.Kill();
	}

	// legacy users / egistered account logon handler
	// > used for registered accounts using 4.61+ client game version
	void OnHandleTokenLogon(GameClient* pClient, const std::string& textPacket)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		TextScanner t;
		t.SetupFromMemoryAddress(textPacket.c_str());
		
		std::string login_token = t.GetParmString("ltoken", 1);
		std::string token_decypher = base64_decode(login_token);
		// now, we have the login info decyphered, we can procceed handling it
		LogMsg("[DEBUG]: login token: %s\n%s", login_token.c_str(), token_decypher.c_str());

		// handling as soon as it's figured out how it works
		t.Kill();
	}


} // namespace GrowPacketsListener

#endif LOGONPACKETLISTENER_H