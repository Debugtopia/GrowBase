#ifndef WORLDSMANAGER_H
#define WORLDSMANAGER_H
#include <string>
#include <vector>

#include <World/World.h>
#include <SDK/Builders/WorldOffersBuilder.h>

// fowarded definitions
class GameClient;

class WorldsManager
{
public:
	WorldsManager() = default;
	~WorldsManager();

	
	// get
	std::vector<World*>          GetActiveWorlds() const { return m_activeWorlds; }

	
	// set


	// fn
	void                         SendWorldOffers(GameClient* pClient, const bool& bOnlineMessage = false);

private:
	std::vector<World*>          m_activeWorlds; // active(loaded) worlds in this server

};

WorldsManager*                   GetWorldsManager();

#endif WORLDSMANAGER_H