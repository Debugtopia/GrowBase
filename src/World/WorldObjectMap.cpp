#include <BaseApp.h> // precompiled

#include <World/WorldObjectMap.h>

#include <SDK/Proton/MiscUtils.h>

WorldObjectMap::~WorldObjectMap()
{
    m_objectID = 0;
    m_objects.clear();
}

WorldObject * WorldObjectMap::GetObjectByID(const int& objectID)
{
    for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject * pObject = &m_objects[i];
		if (pObject == NULL || pObject->ID != objectID)
		{
            // object is null or object ID missmatch
			continue;
		}

		return pObject;
	}

    // no object found
	return NULL;
}

size_t WorldObjectMap::GetMemoryEstimated()
{
    size_t estimated = 0;
    estimated += sizeof(int); // objects size
    estimated += sizeof(int); // last object ID

	for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject * pObject = &m_objects[i];
		if (pObject == NULL)
		{
            // object is null
			continue;
		}

		estimated += pObject->GetMemoryEstimated();
	}

	return estimated;
}

void WorldObjectMap::Reset()
{
    // resets the object map, leaving no floating objects in it
    // @note: keep in mind, that if you use this func in a world with people inside, you'd need to handle visual removal of the objects yourself

    m_objectID = 0;
    m_objects.clear();
}

void WorldObjectMap::Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide)
{
    if (pData == NULL)
    {
        // data is null
        return;
    }

    int objects_size = (int)m_objects.size(); // size of the objects
	int object_offset = bClientSide ? m_objectID - 1 /* last object id */: m_objectID /* current object id */;
	MemorySerializeRaw(objects_size, pData, memOffset, true);
	MemorySerializeRaw(object_offset, pData, memOffset, true);
	
    for (int i = 0; i < m_objects.size(); i++)
	{
		WorldObject * pObject = &m_objects[i];
		if (pObject == NULL)
		{
            // object is null
			continue;
		}

		pObject->Serialize(pData, memOffset);
	}
}

void WorldObjectMap::Load(uint8_t * pData, int& memOffset)
{
    if (pData == NULL)
    {
        // data is null
        return;
    }

    int objects_size = 0;
	MemorySerializeRaw(objects_size, pData, memOffset, false);
	MemorySerializeRaw(m_objectID, pData, memOffset, false);
	
    // loading back the objects from mem
    for (int i = 0; i < objects_size; i++) 
	{
		WorldObject obj;
		obj.Load(pData, memOffset);
        m_objects.push_back(obj);
	}
}