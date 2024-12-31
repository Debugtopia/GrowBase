#include <BaseApp.h> // precompiled

#include <World/WorldObject.h>

#include <SDK/Proton/MiscUtils.h>

size_t WorldObject::GetMemoryEstimated() 
{
	size_t estimated = 0;

	estimated += sizeof(uint16_t); // item ID
	estimated += sizeof(float); // x 
    estimated += sizeof(float); // y
	estimated += sizeof(uint8_t); // item count
	estimated += sizeof(uint8_t); // object flags
	estimated += sizeof(int); // world object offset ID
	return estimated;
}

void WorldObject::Serialize(uint8_t * pData, int& memOffset)
{
	if (pData == NULL)
	{
        // data is null
		return;
	}

	MemorySerializeRaw(itemID, pData, memOffset, true);
	MemorySerializeRaw(x, pData, memOffset, true);
	MemorySerializeRaw(y, pData, memOffset, true);
	MemorySerializeRaw(count, pData, memOffset, true);
	MemorySerializeRaw(flags, pData, memOffset, true);
	MemorySerializeRaw(ID, pData, memOffset, true);
}

void WorldObject::Load(uint8_t * pData, int& memOffset)
{
	if (pData == NULL)
	{
        // data is null
		return;
	}

	MemorySerializeRaw(itemID, pData, memOffset, false);
	MemorySerializeRaw(x, pData, memOffset, false);
	MemorySerializeRaw(y, pData, memOffset, false);
	MemorySerializeRaw(count, pData, memOffset, false);
	MemorySerializeRaw(flags, pData, memOffset, false);
	MemorySerializeRaw(ID, pData, memOffset, false);
}