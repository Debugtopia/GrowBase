#ifndef BASEAPP_H
#define BASEAPP_H
#include <string>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <memory>


#include <GrowConfig.h>

// managers
#include <Items/ItemInfoManager.h>


// global definitions
using nova_str = std::string;
using nova_chr = const char*;
using nova_sstream = std::ostringstream;
using nova_ostream = std::ostringstream;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using ul = unsigned long;
using ull = unsigned long long;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

#define ptr(x) x*

// memory funcs
void nova_memcopy(void* pSourceTo, const void* pSource, size_t sourceSize, int& offset);
void nova_strcopy(void* pSourceTo, const std::string& str, size_t sourceSize, int& offset);


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

	u8            GetServerID() const { return m_serverID; }
	Config        GetConfig() const { return m_config; }

	/*initializes BaseApp, loads essentials and configures the ENet server.*/
	void          Init();

private:
	Config            m_config;

	// file-dependant configs, mostly used for in-game commands like /news, /rules
	nova_str          m_gazette = "";
	nova_str          m_gazette_Android = ""; // this is shown to Android users only
	nova_str          m_rules = "";

	u8                m_serverID = 0; // game server ID
};

BaseApp* GetBaseApp();

#endif BASEAPP_H