#include <BaseApp.h> // precompiled

#include <Client/FunctionFactory.h>
#include <Client/GameClient.h>

FunctionFactory::FunctionFactory(GameClient* pClient)
{
	m_pClient = pClient;
}

void FunctionFactory::OnConsoleMessage(const nova_str& msg, const int& netID, const int& delayMS)
{
	if (m_pClient == NULL)
	{
		// game client is null.
		return;
	}

	VariantList var;
	var.Get(0).Set("OnConsoleMessage");
	var.Get(1).Set(msg);
	m_pClient->SendVariantPacket(var, netID, delayMS);
}

void FunctionFactory::OnRequestWorldSelectMenu(const nova_str& menuString, const int& netID, const int& delayMS)
{
	if (m_pClient == NULL)
	{
		// game client is null.
		return;
	}

	VariantList var;
	var.Get(0).Set("OnRequestWorldSelectMenu");
	var.Get(1).Set(menuString);
	m_pClient->SendVariantPacket(var, netID, delayMS);
}

void FunctionFactory::OnTalkBubble(const int& playerNetID, const nova_str& text, const u8& bubbleType, const bool& bOverrideOld, const int& netID, const int& delayMS)
{
	if (m_pClient == NULL)
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
	m_pClient->SendVariantPacket(var, netID, delayMS);
}