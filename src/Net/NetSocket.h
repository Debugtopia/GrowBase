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


// this key is used to auth sub-servers, if anyone gets access to this key, they can "fake" a server to get data from your server!
// DO NOT SHARE THE AUTH KEY!
#define SERVER_AUTH_KEY "YOUR_SERVER_AUTH_KEY"

// global socket definitions
using ClientSocket = int;
using ServerSocket = int;

#define CLIENT_INVALID_SOCKET -1
#define CLIENT_STATE_DEAD_CONNECTION 0

static void client_socket_close(ClientSocket s)
{
#ifdef _WIN32
    closesocket(s);
    WSACleanup();
#else
    close(s);
#endif
}

typedef struct client_data_t
{
    ClientSocket        socket = CLIENT_INVALID_SOCKET; // this is the socket

    std::string         address = "127.0.0.1"; // this is the address / host used for tcp communication
    uint16_t            tcpCommunicationPort = 18000; // this is the port that is used for tcp communication

    // game server variables
    int                 ID = 0; // game server ID - S0, S1, S2, ...
    int                 enetPort = 17000; // enet server port - 17000, 17001, 17002, 17003, ...

    bool                bQueued = false; // whether the server has been locked out, unable to be switched into due to critical errors
} ClientData;
using NetClients = std::vector<ClientData>;

class NetSocket
{
public:
    NetSocket();
    ~NetSocket();

    // get
    ServerSocket                    GetServerSocket() const { return m_serverSocket; }
    bool                            IsRunningAsServer() const { return m_bRunningAsAServer; }
    NetClients                      GetConnections() const { return m_connections; }
    int                             GetServerOffsetID() const { return m_serverOffset; }
    // set


    // fn
    bool InitServer(const char* pHost, uint16_t tcpPort);
    void Sync();

protected:
    void AcceptConnections();
    void ProccessIncoming();

private:
    ServerSocket                    m_serverSocket = CLIENT_INVALID_SOCKET; // the socket the sub-servers connect to
    bool                            m_bRunningAsAServer = false; // whether the netsocket is used as a server or client

    NetClients                      m_connections = {};
    int                             m_serverOffset = 0;
};

NetSocket*                          GetNetSocket();

#endif NETSOCKET_H