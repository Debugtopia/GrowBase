#ifndef WORLDSMANAGER_H
#define WORLDSMANAGER_H
#include <string>
#include <vector>

#include <SDK/Builders/WorldOffersBuilder.h>

class World;
class GameClient;
typedef std::vector<World*> worlds_array;

class WorldsManager
{
public:
	WorldsManager() = default;
	~WorldsManager();

	
	// get
	worlds_array                 GetActiveWorlds() const { return m_activeWorlds; }

	
	// set


	// fn
	void                         SendWorldOffers(GameClient* pClient, const bool& bOnlineMessage = false);

private:
	worlds_array                 m_activeWorlds; // active(loaded) worlds in this server

};

WorldsManager*                   GetWorldsManager();

#endif WORLDSMANAGER_H