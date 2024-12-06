#ifndef BASEAPP_H
#define BASEAPP_H
#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>


#include <GrowConfig.h>

// managers
#include <Items/ItemInfoManager.h>


// global definitions
#define STR std::string
#define CCH const char*
#define SStream std::ostringstream

#define U8 uint8_t
#define U16 uint16_t
#define U32 uint32_t
#define U64 uint64_t
#define UL unsigned long
#define ULL unsigned long long

#define I8 int8_t
#define I16 int16_t
#define I32 int32_t
#define I64 int64_t

#define byte char
#define __ptr *


/*
This file is precompiled, and required to compile the application.

Please DO NOT remove our credits, we have worked hard to open-source this project and make GTPS community better.
However, you can add yourself to "Additional Credits" category if you like.

Credits:
-  Hexago(Discord, Telegram: xhexago) - main developer of this project
-  Caferius(Discord: caferius) - providing with cache & config files from real servers
-  MuOdO(Discord: muodo) - providing with information over growtopia systems, and many more useful information used in this project

Additional Credits:
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
*/

enum class eLogonMode
{
	// thanks muodo for explaining the logon modes ~ Hexago

	LOGONMODE_NONE = 0, // normal login mode, will show the "Logging on..." messages
	LOGONMODE_SILENT = 1, // switch servers but doesn't shows the "Logging on..." messages
	LOGONMODE_JOINREQUEST, // sends action|join_request with doorID when called

	NUM_LOGONMODE
};

// console log funcs
static std::ofstream g_logFile; // the file we write the logs into
void LogMsg(const char* traceStr, ...);
void LogError(const char* traceStr, ...);


Config GetConfig();
class BaseApp
{
public:
	BaseApp();
	~BaseApp();

	U8            GetServerID() const { return m_serverID; }
	Config        GetConfig() const { return m_config; }

	/*initializes BaseApp, loads essentials and configures the ENet server.*/
	void          Init();

private:
	// file-dependant configs, mostly used for in-game commands like /news, /rules
	std::string       m_gazette = "";
	std::string       m_gazette_Android = ""; // this is shown to Android users only
	std::string       m_rules = "";

	U8                m_serverID = 0; // game server ID
	Config m_config;
};

BaseApp __ptr GetBaseApp();

#endif BASEAPP_H