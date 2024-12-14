#include <BaseApp.h> // precompiled

#include <Server/ENetServer.h>
#include <Client/GameClient.h>

ENetServer g_server;
ENetServer* GetENetServer() { return &g_server; }

ENetServer::~ENetServer()
{
	/* deinitializing the enet */
	enet_deinitialize();
}

void ENetServer::Run(const char* pAddress, uint16_t addressPort)
{
	if (enet_initialize() != 0)
	{
		/* failed to init */
		LogError("failed to initialize enet");
		return;
	}

	if (m_pHost != NULL)
	{
		LogError("failed to start enet server, because one is already running!");
		return;
	}

	/* setting up enet address */
	ENetAddress address;
	address.port = addressPort; // UDP
	m_port = addressPort;
	enet_address_set_host(&address, pAddress);
	/* creating enet host */
	m_pHost = enet_host_create(&address, GetConfig().enetMaxPeers, 10, 0, 0);
	if (m_pHost == NULL)
	{
		/* enet host creation failed, possibly port used? */
		LogError("unable to create server host");
		return;
	}

	//m_handler = PacketHandler(m_pHost);
	m_pHost->checksum = enet_crc32;
	enet_host_compress_with_range_coder(m_pHost);
	LogMsg("enet server serving on %s:%d", pAddress, addressPort);
	m_bRunning = true;
	RunEventListener();
}

void ENetServer::Kill()
{
	LogMsg("killing server...");
	/* disconnecting all connections */
	for (int i = 0; i < m_connections.size(); i++)
	{
		ENetPeer *pConnectionPeer = m_connections[i];
		if (pConnectionPeer == NULL)
		{
			continue;
		}

		enet_peer_disconnect_later(pConnectionPeer, 0U);
	}

	/* flushing to stop current queued packets from coming */
	enet_host_flush(m_pHost);

	/* killing the host */
	enet_host_destroy(m_pHost);

	/* cleans up the global state of resources */
	enet_deinitialize();
	m_bRunning = false;
	LogMsg("server killed");
}

void ENetServer::RunEventListener()
{
	ENetEvent eEvent;
    while (m_bRunning)
    {
        while (enet_host_service(m_pHost, &eEvent, GetConfig().enetTimeout) > 0)
        {
            switch (eEvent.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    if (eEvent.peer->data != NULL)
                    {
                        break;
                    }

					// creating our player object & proceeding to logon
					GameClient * pClient = new GameClient(eEvent.peer);
					pClient->OnConnect();
                    break;
                }

                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    if (eEvent.peer == NULL || !eEvent.peer->data)
                    {
                        break;
                    }

					// deleting player
                    delete eEvent.peer->data;
                    eEvent.peer->data = NULL;
                    break;
                }

                case ENET_EVENT_TYPE_RECEIVE:
                {
                    if (eEvent.peer == NULL || eEvent.peer->data == NULL)
                    {
                        break;
                    }

					// handling incoming packet
                    GetPacketHandler()->HandleIncomingClientPacket(eEvent.peer, eEvent.packet);
                    enet_packet_destroy(eEvent.packet);
                    break;
                }
            }
        }
    }
}