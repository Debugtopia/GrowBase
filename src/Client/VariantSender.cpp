#include <BaseApp.h> // precompiled

#include <Client/VariantSender.h>
#include <Client/GameClient.h>


void VariantSender::OnConsoleMessage(GameClient * pClient, const nova_str& msg, const int& netID, const int& delayMS)
{
	if (pClient == NULL)
	{
		// game client is null.
		return;
	}

	VariantList var;
	var.Get(0).Set("OnConsoleMessage");
	var.Get(1).Set(msg);
	pClient->SendVariantPacket(var, netID, delayMS);
}

void VariantSender::OnRequestWorldSelectMenu(GameClient * pClient, const nova_str& menuString, const int& netID, const int& delayMS)
{
	if (pClient == NULL)
	{
		// game client is null.
		return;
	}

	VariantList var;
	var.Get(0).Set("OnRequestWorldSelectMenu");
	var.Get(1).Set(menuString);
	pClient->SendVariantPacket(var, netID, delayMS);
}

void VariantSender::OnSpawn(GameClient * pClient, const nova_str& spawnData, const int& netID, const int& delayMS)
{
	// default delay for this func is -1
	if (pClient == NULL)
	{
		// game client is null.
		return;
	}

	VariantList var;
	var.Get(0).Set("OnSpawn");
	var.Get(1).Set(spawnData);
	pClient->SendVariantPacket(var, netID, delayMS);
}

void VariantSender::OnTalkBubble(GameClient * pClient, const int& playerNetID, const nova_str& text, const u8& bubbleType, const bool& bOverrideOld, const int& netID, const int& delayMS)
{
	if (pClient == NULL)
	{
		// game client is null.
		return;
	}

	VariantList var;
	var.Get(0).Set("OnTalkBubble");
	var.Get(1).Set(playerNetID);
	var.Get(2).Set(text);
	var.Get(3).Set(bubbleType);
	var.Get(4).Set(bOverrideOld);
	pClient->SendVariantPacket(var, netID, delayMS);
}

void VariantSender::OnDialogRequest(GameClient * pClient, const nova_str& menuString, const int& netID, const int& delayMS)
{
	if (pClient == NULL)
	{
		// game client is null.
		return;
	}

	VariantList var;
	var.Get(0).Set("OnDialogRequest");
	var.Get(1).Set(menuString);
	pClient->SendVariantPacket(var, netID, delayMS);
}