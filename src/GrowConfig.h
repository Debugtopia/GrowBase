#ifndef GROWCONFIG_H
#define GROWCONFIG_H
#include <string>
#include <vector>

struct Config
{
	std::string address;
	uint16_t    logonPort;
	uint16_t    basePort;
	uint16_t    adminPort;


	std::string sqlHost;
	std::string sqlUser;
	std::string sqlPass;
	std::string sqlName;


	std::string downloadServerURL = "";
	std::string downloadServerPath = "";

	bool        bBetaServer = false;
	std::string betaMsg = "";
	std::string illegalAndroidApps = "";


	int         enetTimeout = 5000;
	int         enetMaxPeers = 250;
	int         enetConnectionsPerSec = 30;
	int         enetConnectionsPerIP = 3;


	int         maxPlayersInWorld = 60;
	int         daysToDeleteLock = 179;
	uint8_t     mapVersion = 5;


	double      gemsMultiplier = 1.0;
	double      xpMultiplier = 1.0;

	bool        bDisableGamePack = false;
	bool        bCollidateDrops = false;
	bool        bWorldBalance = true;
	bool        bAutoImport = true;
	bool        bEnableFriends = true;
	bool        bWarpToFriendEnabled = true;

	int         friendsUpdateIntervalMS = 10000;
	int         maxFriends = 200;
	int         maxIgnores = 50;
};

class GrowConfig
{
public:
	GrowConfig() = default;
	~GrowConfig() = default;

	/*loads config.txt*/
	bool       Load(Config& conf);
	/*reloads live-updateable lines in config.txt*/
	bool       Reload(Config& conf);
};

GrowConfig*    GetGrowConfig();

#endif GROWCONFIG_H