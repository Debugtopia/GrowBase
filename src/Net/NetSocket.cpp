#include <BaseApp.h> // precompiled
#include <Net/NetSocket.h>

#include <SDK/Proton/MiscUtils.h>

NetSocket::NetSocket()
{
	//
}

NetSocket::~NetSocket()
{
	if (m_bRunningAsAServer)
	{
		/* killing net server*/
		client_socket_close(m_serverSocket);
	}
}

/*initializes netserver
> pHost -> the tcp ip address the server is hosted on
> tcpPort -> the tcp port used for connecting or communicating with the server*/
bool NetSocket::InitServer(const char* pHost, uint16_t tcpPort)
{
	if (m_serverSocket != CLIENT_INVALID_SOCKET)
	{
		LogError("failed to init NetServer, because there already is one running!");
		return false;
	}

	m_bRunningAsAServer = true;
#ifdef _WIN32
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
	{
		LogError("failed to setup wsa data, exception: %s", res);
		return false;
	}
#endif

	m_serverSocket = (ClientSocket)socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverSocket == CLIENT_INVALID_SOCKET)
	{
		LogError("failed to init NetServer, error trying to create the server socket.");
		client_socket_close(m_serverSocket);
		return false;
	}

	// setting up socket address
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(tcpPort);
	addr.sin_addr.s_addr = INADDR_ANY;

	// binding the server to the address
	if (bind(m_serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		LogError("failed to init NetServer, error trying binding the address.");
		client_socket_close(m_serverSocket);
		return false;
	}

	if (listen(m_serverSocket, 100) < 0)
	{
		LogError("failed to init NetServer, error setting up a listener.");
		client_socket_close(m_serverSocket);
		return false;
	}

	LogMsg("NetServer serving on %s:%d", pHost, tcpPort);
	return true;
}

void NetSocket::Sync()
{
	if (!m_bRunningAsAServer)
	{
		// client cannot re-sync
		return;
	}

	// resyncing means we send an update status packet, with the list of active connections in the server, every game server gets it updated when needed.
	// that's how we keep track of the game servers in the entire network(some of them are on different machines than others)
	size_t resyncPacketLen = 4;
	int offset = 0;
	for (int i = 0; i < m_connections.size(); i++)
	{
		ClientData client = m_connections[i];
		resyncPacketLen += sizeof(ClientSocket);
		resyncPacketLen += 2 + client.address.length();
		resyncPacketLen += sizeof(u16); // tcpCommunicationPort

		resyncPacketLen += sizeof(int); // ID
		resyncPacketLen += sizeof(int); // enet server port

		resyncPacketLen += sizeof(bool); // bQueued
	}

	GrowPacket *pGrowResyncPacket = (GrowPacket*)std::malloc(sizeof(GrowPacket) + resyncPacketLen);
	if (pGrowResyncPacket == NULL)
	{
		// failed allocating data for pGrowtErrorPacket, well... since nova_dealloc checks for null pointers to prevent crashs upon free-ing, we won't have issues freeing it for security anyways.
		nova_dealloc(pGrowResyncPacket);
		return;
	}

	pGrowResyncPacket->type = NET_GROW_PACKET_UPDATE_STATUS;
	pGrowResyncPacket->flags = NET_GROW_PACKET_FLAG_EXTENDED_DATA;
	pGrowResyncPacket->dataLength = (uint32_t)resyncPacketLen;

	// writing connections data to the packet
	int connectionsCount = m_connections.size();
	MemorySerializeRaw(connectionsCount, pGrowResyncPacket->data, offset, true);
	for (int i = 0; i < m_connections.size(); i++)
	{
		ClientData client = m_connections[i];
		MemorySerializeRaw(client.socket, pGrowResyncPacket->data, offset, true);
		MemorySerialize(client.address, pGrowResyncPacket->data, offset, true);
		MemorySerializeRaw(client.tcpCommunicationPort, pGrowResyncPacket->data, offset, true);
		MemorySerializeRaw(client.ID, pGrowResyncPacket->data, offset, true);
		MemorySerializeRaw(client.enetPort, pGrowResyncPacket->data, offset, true);
		MemorySerializeRaw(client.bQueued, pGrowResyncPacket->data, offset, true);
	}

	// sending the packet to all connections
	for (int i = 0; i < m_connections.size(); i++)
	{
		ClientData client = m_connections[i];
		// send the packet
	}

	nova_dealloc(pGrowResyncPacket); // dealloc the packet to not leak memory
}

void NetSocket::AcceptConnections()
{
	while (true)
	{
		if (m_bRunningAsAServer)
		{
			// server handling packets
			sockaddr_in addr;
			socklen_t size = sizeof(addr);
			ClientSocket clientSocket = (ClientSocket)accept(m_serverSocket, (struct sockaddr*)&addr, &size);
			if (clientSocket == CLIENT_INVALID_SOCKET)
			{
				LogError("accepted client socket was not valid? Something went wrong!");
				client_socket_close(clientSocket);
				return;
			}

			// creating client data object to store inside connections
			ClientData client;
			client.socket = clientSocket;
			client.enetPort = GetConfig().basePort + m_serverOffset;
			client.tcpCommunicationPort = GetConfig().basePort + 1000 + m_serverOffset;
			client.bQueued = false;
			++m_serverOffset;

			char buffer[1024] = { 0 };
			int bufferLength = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bufferLength < 1)
			{
				// received data was empty
				client_socket_close(clientSocket);
				return;
			}

			if (bufferLength < 2)
			{
				// packet is missing important header
				LogError("a socket tried to send a packet, unfortunately it didn't pass the safety checks due to missing header.");
				client_socket_close(clientSocket);
				return;
			}

			// getting auth token
			nova_str authToken = nova_str(buffer, bufferLength);
			nova_str rawToken = base64_decode(authToken); 			// decyphered auth token
			nova_stringarr tokenizedAuth = Utils::StringTokenize(rawToken, "|");
			if (tokenizedAuth.size() < 2)
			{
				// auth token did not contain needed information
				nova_str reply = "Ouch! That did not work, good luck, pleasant...";
				size_t replyPacketLen = 2 + reply.length();
				int offset = 0;

				GrowPacket *pGrowErrorPacket = (GrowPacket*)std::malloc(sizeof(GrowPacket) + replyPacketLen);
				if (pGrowErrorPacket == NULL)
				{
					// failed allocating data for pGrowtErrorPacket, well... since nova_dealloc checks for null pointers to prevent crashs upon free-ing, we won't have issues freeing it for security anyways.
					nova_dealloc(pGrowErrorPacket);
					return;
				}

				pGrowErrorPacket->type = NET_GROW_PACKET_CONSOLE_OUTPUT;
				MemorySerialize(reply, pGrowErrorPacket->data, offset, true);

				// TODO: SendPacket function
				nova_dealloc(pGrowErrorPacket);
				client_socket_close(clientSocket);
				return;
			}

			client.address = tokenizedAuth[0];
			nova_str authPassword = tokenizedAuth[1];
			if (authPassword != SERVER_AUTH_KEY)
			{
				// auth password incorrect
				nova_str reply = "Ouch! That did not work, good luck, pleasant...";
				size_t replyPacketLen = 2 + reply.length();
				int offset = 0;

				GrowPacket* pGrowErrorPacket = (GrowPacket*)std::malloc(sizeof(GrowPacket) + replyPacketLen);
				if (pGrowErrorPacket == NULL)
				{
					// failed allocating data for pGrowtErrorPacket, well... since nova_dealloc checks for null pointers to prevent crashs upon free-ing, we won't have issues freeing it for security anyways.
					nova_dealloc(pGrowErrorPacket);
					return;
				}

				pGrowErrorPacket->type = NET_GROW_PACKET_CONSOLE_OUTPUT;
				MemorySerialize(reply, pGrowErrorPacket->data, offset, true);

				// TODO: SendPacket function
				nova_dealloc(pGrowErrorPacket);
				client_socket_close(clientSocket);
				return;
			}

			LogMsg("game server S%d connected to network, address: %s:%d", client.ID, client.address.c_str(), client.tcpCommunicationPort);
			GrowPacket netPacket;
			netPacket.type = NET_GROW_PACKET_INIT_HOST;
			netPacket.serverID = client.ID;
			netPacket.tcpCommunicationPort = client.tcpCommunicationPort;
			netPacket.enetPort = client.enetPort;


			m_connections.emplace_back(client);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		else
		{
			// client handling packets
		}
	}
}