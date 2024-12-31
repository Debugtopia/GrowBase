#ifndef VARIANTSENDER_H
#define VARIANTSENDER_H
#include <string>

// fowardeed definitions
class GameClient;

class VariantSender
{
public:
	VariantSender() = default;
	~VariantSender() = default;

	// variants play major part in client's understanding
	// they have specific name, structure, if it's wrong, the client won't understand it
	// here we list the known variants we know for easier usage

	static void OnConsoleMessage(GameClient * pClient, const nova_str& msg, const int& netID = -1, const int& delayMS = 0);
	static void OnRequestWorldSelectMenu(GameClient * pClient, const nova_str& menuString, const int& netID = -1, const int& delayMS = 0);
	static void OnSpawn(GameClient * pClient, const nova_str& spawnData, const int& netID = -1, const int& delayMS = -1);
	static void OnTalkBubble(GameClient * pClient, const int& playerNetID, const nova_str& text, const u8& bubbleType = 0, const bool& bOverrideOld = false, const int& netID = -1, const int& delayMS = 0);
	static void OnDialogRequest(GameClient * pClient, const nova_str& menuString, const int& netID = -1, const int& delayMS = 0);
};

#endif VARIANTSENDER_H