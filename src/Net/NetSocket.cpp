#include <BaseApp.h> // precompiled
#include <Net/NetSocket.h>

#include <SDK/Proton/MiscUtils.h>

NetSocket g_net;
NetSocket* GetNetSocket() { return &g_net; }

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
		LogError("failed to setup wsa data, exception type: %d", res);
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
	// find out how syncing works
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

			LogMsg("game server S%d is trying to connect to network, address: %s:%d", client.ID, client.address.c_str(), client.tcpCommunicationPort);

			// auth packet response we send to client
			GrowPacket netPacket;
			netPacket.type = NET_GROW_PACKET_INIT_HOST;
			netPacket.serverID = client.ID;
			netPacket.tcpCommunicationPort = client.tcpCommunicationPort;
			netPacket.enetPort = client.enetPort;

			// figure how to validate & authenticate client

			m_connections.emplace_back(client);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		else
		{
			// client handling packets
		}
	}
}

void NetSocket::ProccessIncoming()
{
	while (true)
	{
		if (m_bRunningAsAServer)
		{
			// handle packets here as logon-server
		}
		else
		{
			// handle packets here as sub-server
		}
	}
}