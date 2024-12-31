#include <BaseApp.h> // precompiled
#include <World/TileExtra.h>

size_t TileExtraDoor::GetMemoryEstimated(const bool& bClientSide, const float& fClientVersion, const uint16_t& worldMapVersion)
{
    size_t estimated = 0;
	estimated += sizeof(uint16_t) + Label.length();
	estimated += sizeof(uint8_t); //door flags

	if (bClientSide == false)
	{
		estimated += sizeof(uint16_t) + UniqueID.length();
		estimated += sizeof(uint16_t) + Destination.length();
		estimated += sizeof(uint16_t) + Password.length();
	}

	return estimated;
}

void TileExtraDoor::Serialize(uint8_t* pData, int& memOffset, const bool& bClientSide, const float& fClientVersion, const uint16_t& worldMapVersion)
{
    if (pData == NULL)
	{
        // data is null
		return;
	}

	MemorySerialize(Label, pData, memOffset, true);
	MemorySerializeRaw(Flag, pData, memOffset, true);
	if (bClientSide == false)
	{
        // server side infos
		MemorySerialize(UniqueID, pData, memOffset, true);
		MemorySerialize(Destination, pData, memOffset, true);
		MemorySerialize(Password, pData, memOffset, true);
	}
}

void TileExtraDoor::Load(uint8_t* pData, int& memOffset, const bool& bClientSide, const uint16_t& worldMapVersion)
{
    if (pData == NULL)
	{
        // data is null
		return;
	}

	MemorySerialize(Label, pData, memOffset, false);
	MemorySerializeRaw(Flag, pData, memOffset, false);
	if (bClientSide == false)
	{
        // server side infos
		MemorySerialize(UniqueID, pData, memOffset, false);
		MemorySerialize(Destination, pData, memOffset, false);
		MemorySerialize(Password, pData, memOffset, false);
	}
}