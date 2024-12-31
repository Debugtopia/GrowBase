#ifndef WORLDSMANAGER_H
#define WORLDSMANAGER_H
#include <string>
#include <vector>

#include <World/World.h>
#include <SDK/Builders/WorldOffersBuilder.h>
#include <SDK/Builders/DialogBuilder.h>

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
	void                         SendWorldOffers(GameClient * pClient, const bool& bOnlineMessage = false);
	bool                         Enter(GameClient * pClient, const char * fName, CL_Vec2f spawnPoint = CL_Vec2f(0.f, 0.f));

private:
	std::vector<World*>          m_activeWorlds; // active(loaded) worlds in this server

};

WorldsManager*                   GetWorldsManager();

#endif WORLDSMANAGER_H