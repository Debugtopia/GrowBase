#ifndef JOINREQUESTLISTENER_H
#define JOINREQUESTLISTENER_H
#include <string>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

namespace GrowPacketsListener
{
	// sends the client the world selection menu & gazette
	void OnHandleJoinRequest(GameClient* pClient, const char * pChar)
	{
		if (pClient == NULL)
		{
			// game client is null
			return;
		}

		TextScanner t;
        t.SetupFromMemoryAddress(pChar);
        nova_str world_name = Utils::StringUppercase(t.GetParmString("name", 1));
        nova_str world_to_enter = world_name;
        if (world_name.empty())
	    {
		    world_to_enter = "START";
	    }

        bool bIsCensored = Utils::ContainsGTSwear(world_name);
        if (world_name == "EXIT")
	    {
		    pClient->SendEntryFail("Exit from what? Press back if  you're done playing.");
		    return;
	    }

        if (!Utils::IsOnlyAlphabet(world_name))
        {
            pClient->SendEntryFail("Sorry, spaces and special characters are not allowed in world or door names. Try again.");
		    return;
        }

        if (bIsCensored)
        {
            // contains swear in it
            world_to_enter = "DISNEYLAND";
        }

        GetWorldsManager()->Enter(pClient, world_to_enter.c_str());
		t.Kill();
	}

} // namespace GrowPacketsListener

#endif JOINREQUESTLISTENER_H