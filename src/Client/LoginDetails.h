#ifndef LOGINDETAILS_H
#define LOGINDETAILS_H
#include <string>
#include <cstdint>

enum ePlatformID
{
	PLATFORM_ID_UNKNOWN = -1,
	PLATFORM_ID_WINDOWS,
	PLATFORM_ID_IOS, // iPhone/iPad etc
	PLATFORM_ID_OSX,
	PLATFORM_ID_LINUX,
	PLATFORM_ID_ANDROID,
	PLATFORM_ID_WINDOWS_MOBILE, // yeah, right.  Doesn't look like we'll be porting here anytime soon.
	PLATFORM_ID_WEBOS,
	PLATFORM_ID_BBX, // RIM Playbook
	PLATFORM_ID_FLASH,
	PLATFORM_ID_HTML5, // javascript output via emscripten for web
	PLATFORM_ID_PSVITA,

	// new platforms will be added above here.  Don't count on PLATFORM_ID_COUNT not changing!
	PLATFORM_ID_COUNT
};

class LoginDetails
{
public:
	LoginDetails() = default;
	~LoginDetails() = default;

	char address[16] = "127.0.0.1"; // 16

	std::string requestedName = "";
	std::string tankIDName = "";
	// tankIDPass is not stored, anywhere, we store the hash of it here.
	// the hash is ENCRYPTED and cannot be decrypted, not even by us!
	std::string hash = "";

	std::string platformID = "4";
	float gameVersion = 2.998f; // 5

	std::string country = "us"; // 2

	// randomID - generated upon client installation
	// used to authenticate guest users
	std::string rid = ""; // 24
	
	// mac(media access control) - it's the IP address's mac address
	// the mac address of 02:00:00:00:00:00 is blocked on windows OS when logging in the game.
	std::string mac = "02:00:00:00:00:00"; // 17

	// logon mode is used when sending OnSendToServer to switch the player between 2 sub-servers
	int logonMode = 0;

	// doorID is the world the player is supposte to enter upon switch of servers
	std::string doorID = "EXIT";
};

#endif LOGINDETAILS_H