#ifndef NETSOCKET_H
#define NETSOCKET_H
#include <string>
#include <vector>
#include <mutex>

// sockets dependicies here
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

#include <Net/GrowPacket.h>

using ClientSocket = int;
using ServerSocket = int;
using NetClients = std::vector<ClientData>;
static void client_socket_close(ClientSocket s)
{
#ifdef _WIN32
    closesocket(s);
    WSACleanup();
#else
    close(ice);
#endif
}

struct ClientData
{
    ClientSocket socket; // this is the socket

    std::string address = "127.0.0.1"; // this is the address / host used for tcp communication
    short tcpCommunicationPort = 18000; // this is the port that is used for tcp communication

    // game server variables
    int ID = 0; // game server ID - S0, S1, S2, ...
    int enetPort = 17000; // enet server port - 17000, 17001, 17002, 17003, ...

    bool bDisabled = false; // whether the server has been locked out, unable to be switched into due to critical errors
};

class NetSocket
{
public:
    NetSocket();
    ~NetSocket();

    // get
    ServerSocket                    *GetServerSocket() const { return m_pServerSocket; }
    bool                            IsRunningAsServer() const { return m_bRunningAsAServer; }
    NetClients                      GetConnections() const { return m_connections; }
    // set


    // fn

private:
    ServerSocket                    *m_pServerSocket = NULL; // the socket the sub-servers connect to
    bool                            m_bRunningAsAServer = false; // whether the netsocket is used as a server or client

    NetClients                      m_connections = {};
};

#endif NETSOCKET_H