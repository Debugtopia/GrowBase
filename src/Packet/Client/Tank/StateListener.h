#ifndef STATELISTENER_H
#define STATELISTENER_H
#include <string>

#include <SDK/Proton/TextScanner.h>
#include <SDK/Proton/MiscUtils.h>

namespace TankPacketsListener
{
	// sends the client the world selection menu & gazette
	void OnHandleStatePacket(GameClient* pClient, GameUpdatePacket * pTankPacket)
	{
		if (pClient == NULL || pTankPacket == NULL)
		{
			// game client is null
			return;
		}

		World * pWorld = pClient->GetWorld();
		if (pWorld == NULL)
		{
            // world was null(player is not in world)
			return;
		}

        //pClient->ToggleTempBit(PLAYER_TEMP_FLAG_FACING_LEFT, pTankPacket->flags& NET_GAME_PACKET_FLAG_FACING_LEFT);
        int destX = static_cast<int>(pTankPacket->vecX / 32.f);
		int destY = static_cast<int>(pTankPacket->vecY / 32.f);
        if (destX < 0 || destY < 0 || destX >= pWorld->GetWorldTileMap()->GetWidth() || destY >= pWorld->GetWorldTileMap()->GetHeight())
		{
            // moving out of border? what the fuck are u doing bro...
            // i guess you coulda add some whacky warning here to detect cheaters!1!1!1
			return;
        }

        // i guess you could lower it if u find neccessary, the values i set were tested in nova
        if (pTankPacket->speedOut < -800.f || pTankPacket->speedOut > 800.f || pTankPacket->speedIn < -850.f || pTankPacket->speedIn > 850.f)
		{
            // some speed hack type shi
            // i guess you coulda add some whacky warning here to detect cheaters!1!1!1
            pClient->SendVariantPacket({ "OnSetPos", pClient->GetPosition() }, pClient->GetNetID());
            return;
        }

        // these vectors are used for teleport cheat detection and pathfinding
        CL_Vec2i start = CL_Vec2i(static_cast<int>((pClient->GetPosition().X + 10) / 32), static_cast<int>((pClient->GetPosition().Y + 15) / 32));
		CL_Vec2i goal = CL_Vec2i(static_cast<int>((pTankPacket->vecX + 10) / 32), static_cast<int>((pTankPacket->vecY + 15) / 32));

        // i guess you could lower it if u find neccessary, the values i set were tested in nova
        if (std::abs(goal.X - start.X) >= 8 || std::abs(start.Y - goal.Y) >= 8 || std::abs(goal.X - start.X) <= -8 || std::abs(start.Y - goal.Y) <= -8)
		{
            // the kid is trying to teleport, fuck his ass outta here
			pClient->SendVariantPacket({ "OnSetPos", pClient->GetPosition() }, pClient->GetNetID());
			return;
		}

        pTankPacket->netID = pClient->GetNetID();
		pClient->SetPosition(pTankPacket->vecX, pTankPacket->vecY);

        pWorld->Broadcast([&](GameClient * pPlayer) {
			pPlayer->SendPacketRaw(NET_MESSAGE_GAME_PACKET, pTankPacket, sizeof(GameUpdatePacket) + pTankPacket->dataLength, ENET_PACKET_FLAG_RELIABLE);
		});
	}

} // namespace TankPacketsListener

#endif STATELISTENER_H