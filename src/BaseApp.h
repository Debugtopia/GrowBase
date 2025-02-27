#ifndef BASEAPP_H
#define BASEAPP_H
#include <string>
#include <chrono>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>
#include <thread>



#include <GrowConfig.h>



// managers
#include <Items/ItemInfoManager.h>
#include <World/WorldsManager.h>


// global definitions
using nova_str =       std::string;
using nova_chr =       const char*;
using nova_sstream =   std::ostringstream;
using nova_ostream =   std::ostringstream;
using nova_fstream =   std::fstream;
using nova_io =        std::ios;
using nova_stringarr = std::vector<nova_str>;
using nova_clock =     std::chrono::steady_clock;
using nova_clock_sys = std::chrono::system_clock;
using nova_highres_clock = std::chrono::high_resolution_clock;

using u8 =             uint8_t;
using u16 =            uint16_t;
using u32 =            uint32_t;
using u64 =            uint64_t;
using ul =             unsigned long;
using ull =            unsigned long long;

using i8 =             int8_t;
using i16 =            int16_t;
using i32 =            int32_t;
using i64 =            int64_t;

#define                ptr(x) x*



// memory funcs
void   nova_memcopy(void* pSourceTo, const void* pSource, size_t sourceSize, int& offset);
void   nova_strcopy(void* pSourceTo, const std::string& str, size_t sourceSize, int& offset);
void   nova_dealloc(void* pSource);
void   nova_delete(void* pSource);
void   nova_delete_arr(void* pSource);



/*
+ This file is precompiled, and required to compile the application.

Please DO NOT remove our credits, we have worked hard to open-source this project and make GTPS community better.
- However, you can add yourself to "Additional Credits" category if you like.

Feel free to dm xhexago on discord for any questions regarding the source
- Official Discord Server for this open-source project: https://discord.gg/z27evDGqhy

Official Contributors Credits:
-  Hexago(Discord, Telegram: xhexago) - main developer of this project
-  Caferius(Discord: caferius) - providing with cache & config files from real servers
-  MuOdO(Discord: muodo) - providing with information over growtopia systems, and many more useful information used in this project


Unofficial Contributors Credits:
-  dos(Discord: datdos) - provided with some insider info, helped with variables naming and improving the code style
-  Rebillion(Discord: rebillionz) - provided with config files, explanation for some of the keys, helped me get in the gt coding back in 2020


Additional Credits:
-  YoruAkio(discord: yoruakio) - helped with testing growbase's linux support
-  kevz(discord: zkevz) - Broadcast function & gameupdatepacket struct from beef, pushed me to want to learn more about programming
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
-  add yourself here
*/



// global variables
static  std::ofstream           g_logFile; // the file we write the logs into




enum class eLogonMode
{
	// thanks muodo for explaining the logon modes ~ Hexago
	LOGONMODE_NONE = 0, // normal login mode, will show the "Logging on..." messages
	LOGONMODE_SILENT = 1, // switch servers but doesn't shows the "Logging on..." messages
	LOGONMODE_JOINREQUEST, // sends action|join_request with doorID when called

	NUM_LOGONMODE
};



// console log funcs
void   LogMsg(const char* traceStr, ...);
void   LogError(const char* traceStr, ...);

// log funcs
void   LogMsgSafe(const char* traceStr, ...);
void   LogErrorSafe(const char* traceStr, ...);

Config GetConfig();
bool   IsBeta();


class BaseApp
{
public:
	BaseApp();
	~BaseApp();

	u8                GetServerID() const { return m_serverID; }
	Config            GetConfig() const { return m_config; }

	/*initializes BaseApp, loads essentials and configures the ENet server.*/
	void              Init();

private:
	Config            m_config;

	// file-dependant configs, mostly used for in-game commands like /news, /rules
	nova_str          m_gazette = "";
	nova_str          m_gazette_Android = ""; // this is shown to Android users only
	nova_str          m_rules = "";

	u8                m_serverID = 0; // game server ID
};

BaseApp*              GetBaseApp();

#endif BASEAPP_H