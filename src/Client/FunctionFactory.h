#ifndef VARIANTFUNC_H
#define VARIANTFUNC_H
#include <string>

// fowardeed definitions
class GameClient;

class FunctionFactory
{
public:
	FunctionFactory(GameClient* pClient);
	~FunctionFactory() = default;

	// variants play major part in client's understanding
	// they have specific name, structure, if it's wrong, the client won't understand it
	// here we list the known variants we know for easier usage

	void OnConsoleMessage(const nova_str& msg, const int& netID = -1, const int& delayMS = 0);
	void OnRequestWorldSelectMenu(const nova_str& menuString, const int& netID = -1, const int& delayMS = 0);
	void OnTalkBubble(const int& playerNetID, const nova_str& text, const u8& bubbleType = 0, const bool& bOverrideOld = false, const int& netID = -1, const int& delayMS = 0);

private:
	GameClient* m_pClient = NULL;

};

#endif VARIANTFUNC_H