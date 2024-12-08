#ifndef ENETSERVER_H
#define ENETSERVER_H
#include <cstdint>
#include <vector>

#include <enet/enet.h>
#include <Server/PacketHandler.h>


class ENetServer
{
public:
	ENetServer() = default;
	~ENetServer();

	ENetHost*    GetHostPtr() const { return m_pHost; }
	bool         IsRunning() const { return m_bRunning; }
	uint16_t     GetPort() const { return m_port; }
	std::vector<ENetPeer *> GetConnections() const { return m_connections; }

	void Run(const char* pAddress, uint16_t addressPort);
	void Kill();
	void RunEventListener();

private:
	ENetHost *   m_pHost = NULL;
	uint16_t     m_port = 17000;
	bool m_bRunning = false;

	std::vector<ENetPeer *> m_connections;
};

ENetServer* GetENetServer();

#endif ENETSERVER_H