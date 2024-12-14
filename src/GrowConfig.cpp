#include <BaseApp.h> // precompiled
#include <GrowConfig.h>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

GrowConfig g_config;
GrowConfig * GetGrowConfig() { return &g_config; }

bool GrowConfig::Load(Config& conf)
{
	TextScanner t;
	t.LoadFile("config.txt");
	if (!t.IsLoaded())
	{
		LogError("failed to load config.txt");
		return false;
	}

	conf.logonPort = t.GetParmInt("logon_port", 1);
	conf.basePort = t.GetParmInt("base_port", 1);
	conf.logonPort = t.GetParmInt("logon_port", 1);

	conf.sqlUser = t.GetParmString("sql_logon", 1);
	conf.sqlPass = t.GetParmString("sql_password", 1);
	conf.sqlHost = t.GetParmString("sql_host", 1);
	conf.sqlName = t.GetParmString("sql_db_name_to_use", 1);

	conf.betaMsg = t.GetParmString("enable_beta_message", 1);
	conf.bBetaServer = !conf.betaMsg.empty();
	
	conf.enetMaxPeers = t.GetParmInt("max_clients", 1);
	conf.maxPlayersInWorld = t.GetParmInt("max_clients_per_world", 1);
	conf.daysToDeleteLock = t.GetParmInt("days_required_to_delete_lock", 1);
	conf.bDisableGamePack = (bool)t.GetParmInt("disable_gamepack", 1);
	conf.bCollidateDrops = (bool)t.GetParmInt("consolidate_drops", 1);
	conf.bWorldBalance = (bool)t.GetParmInt("world_balance", 1);
	conf.bAutoImport = (bool)t.GetParmInt("auto_import", 1);
	conf.bEnableFriends = (bool)t.GetParmInt("enable_friends", 1);
	conf.bWarpToFriendEnabled = (bool)t.GetParmInt("warp_to_friend_enabled", 1);

	conf.friendsUpdateIntervalMS = t.GetParmInt("friend_update_interval_ms", 1);
	conf.maxFriends = t.GetParmInt("max_friends", 1);
	conf.maxIgnores = t.GetParmInt("max_ignores", 1);
	conf.enetTimeout = t.GetParmInt("enet_loop_timeout", 1);

	std::vector<nova_str> lines = t.GetLines();
	for (int i = 0; i < lines.size(); i++)
	{
		const std::string& line = lines[i];
		if (line.starts_with('#') || line.empty())
		{
			continue;
		}

		std::vector<nova_str> tokens = Utils::StringTokenize(line);
		if (tokens[0] == "downloadServerURL")
		{
			if (tokens.size() < 1)
			{
				continue;
			}

			conf.downloadServerURL = tokens[1];
		}

		if (tokens[0] == "downloadServerPath")
		{
			if (tokens.size() < 1)
			{
				continue;
			}

			conf.downloadServerPath = tokens[1] + "cache/";
		}
	}

	LogMsg("loaded config.txt");
	t.Kill();
	return true;
}

bool GrowConfig::Reload(Config& conf)
{
	TextScanner t;
	t.LoadFile("config.txt");
	if (!t.IsLoaded())
	{
		LogError("failed to reload config.txt");
		return false;
	}

	conf.maxPlayersInWorld = t.GetParmInt("max_clients_per_world", 1);
	conf.daysToDeleteLock = t.GetParmInt("days_required_to_delete_lock", 1);
	conf.bDisableGamePack = (bool)t.GetParmInt("disable_gamepack", 1);
	conf.bCollidateDrops = (bool)t.GetParmInt("consolidate_drops", 1);
	conf.bWorldBalance = (bool)t.GetParmInt("world_balance", 1);
	conf.bAutoImport = (bool)t.GetParmInt("auto_import", 1);
	conf.bEnableFriends = (bool)t.GetParmInt("enable_friends", 1);
	conf.bWarpToFriendEnabled = (bool)t.GetParmInt("warp_to_friend_enabled", 1);

	conf.maxFriends = t.GetParmInt("max_friends", 1);
	conf.maxIgnores = t.GetParmInt("max_ignores", 1);

	LogMsg("reloaded config.txt");
	t.Kill();
	return true;
}