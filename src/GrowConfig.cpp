#include <BaseApp.h> // precompiled
#include <GrowConfig.h>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

GrowConfig g_config;
GrowConfig __ptr GetGrowConfig() { return &g_config; }

bool GrowConfig::Load()
{
	TextScanner t;
	t.LoadFile("config.txt");
	if (!t.IsLoaded())
	{
		LogError("failed to load config.txt");
		return false;
	}

	m_config.logonPort = t.GetParmInt("logon_port", 1);
	m_config.basePort = t.GetParmInt("base_port", 1);
	m_config.logonPort = t.GetParmInt("logon_port", 1);

	m_config.sqlUser = t.GetParmString("sql_logon", 1);
	m_config.sqlPass = t.GetParmString("sql_password", 1);
	m_config.sqlHost = t.GetParmString("sql_host", 1);
	m_config.sqlName = t.GetParmString("sql_db_name_to_use", 1);

	m_config.betaMsg = t.GetParmString("enable_beta_message", 1);
	m_config.bBetaServer = !m_config.betaMsg.empty();
	
	m_config.enetMaxPeers = t.GetParmInt("max_clients", 1);
	m_config.maxPlayersInWorld = t.GetParmInt("max_clients_per_world", 1);
	m_config.daysToDeleteLock = t.GetParmInt("days_required_to_delete_lock", 1);
	m_config.bDisableGamePack = (bool)t.GetParmInt("disable_gamepack", 1);
	m_config.bCollidateDrops = (bool)t.GetParmInt("consolidate_drops", 1);
	m_config.bWorldBalance = (bool)t.GetParmInt("world_balance", 1);
	m_config.bAutoImport = (bool)t.GetParmInt("auto_import", 1);
	m_config.bEnableFriends = (bool)t.GetParmInt("enable_friends", 1);
	m_config.bWarpToFriendEnabled = (bool)t.GetParmInt("warp_to_friend_enabled", 1);

	m_config.friendsUpdateIntervalMS = t.GetParmInt("friend_update_interval_ms", 1);
	m_config.maxFriends = t.GetParmInt("max_friends", 1);
	m_config.maxIgnores = t.GetParmInt("max_ignores", 1);
	m_config.enetTimeout = t.GetParmInt("enet_loop_timeout", 1);

	std::vector<STR> lines = t.GetLines();
	for (int i = 0; i < lines.size(); i++)
	{
		const std::string& line = lines[i];
		if (line.starts_with('#') || line.empty())
		{
			continue;
		}

		std::vector<STR> tokens = Utils::StringTokenize(line);
		if (tokens[0] == "downloadServerURL")
		{
			if (tokens.size() < 1)
			{
				continue;
			}

			m_config.downloadServerURL = tokens[1];
		}

		if (tokens[0] == "downloadServerPath")
		{
			if (tokens.size() < 1)
			{
				continue;
			}

			m_config.downloadServerPath = tokens[1];
		}


	}

	LogMsg("loaded config.txt");
	t.Kill();
	return true;
}

bool GrowConfig::Reload()
{
	TextScanner t;
	t.LoadFile("config.txt");
	if (!t.IsLoaded())
	{
		LogError("failed to reload config.txt");
		return false;
	}

	m_config.maxPlayersInWorld = t.GetParmInt("max_clients_per_world", 1);
	m_config.daysToDeleteLock = t.GetParmInt("days_required_to_delete_lock", 1);
	m_config.bDisableGamePack = (bool)t.GetParmInt("disable_gamepack", 1);
	m_config.bCollidateDrops = (bool)t.GetParmInt("consolidate_drops", 1);
	m_config.bWorldBalance = (bool)t.GetParmInt("world_balance", 1);
	m_config.bAutoImport = (bool)t.GetParmInt("auto_import", 1);
	m_config.bEnableFriends = (bool)t.GetParmInt("enable_friends", 1);
	m_config.bWarpToFriendEnabled = (bool)t.GetParmInt("warp_to_friend_enabled", 1);

	m_config.maxFriends = t.GetParmInt("max_friends", 1);
	m_config.maxIgnores = t.GetParmInt("max_ignores", 1);

	std::vector<STR> lines = t.GetLines();
	for (int i = 0; i < lines.size(); i++)
	{
		const std::string& line = lines[i];
		if (line.starts_with('#') || line.empty())
		{
			continue;
		}

		std::vector<STR> tokens = Utils::StringTokenize(line);
	}

	LogMsg("reloaded config.txt");
	t.Kill();
	return true;
}