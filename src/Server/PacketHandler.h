#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H
#include <string>

#include <enet/enet.h>
#include <Packet/GameUpdatePacket.h>

class PacketHandler
{
public:
    PacketHandler() = default;
    PacketHandler(ENetHost* pHost);


    void        HandleIncomingClientPacket(ENetPeer* pConnectionPeer, ENetPacket* pPacket);

private:
    ENetHost    *m_pHost = NULL;
};

PacketHandler*  GetPacketHandler();

#endif PACKETHANDLER_H