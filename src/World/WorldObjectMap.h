#ifndef WORLDOBJECTMAP_H
#define WORLDOBJECTMAP_H
#include <string>
#include <vector>

#include <World/WorldObject.h>

#include <SDK/Proton/Math.h>


class WorldObjectMap 
{
public:
	WorldObjectMap() = default;
	~WorldObjectMap();


    // get
	int                               GetObjectID(const bool& bIncrease = false) { return bIncrease ? m_objectID++ : m_objectID; }
	WorldObject                       *GetObjectByID(const int& objectID);
	std::vector<WorldObject>          GetObjects() { return m_objects; }
	size_t                            GetMemoryEstimated();

    // set
    void                              SetObjectID(const int& objectID) { m_objectID = objectID; }

    // fn
	void                              Reset();

	void                              AddObject(WorldObject& object);
	void                              RemoveObjectByID(const int& ID);

	void                              Serialize(uint8_t * pData, int& memOffset, const bool& bClientSide = true);
	void                              Load(uint8_t * pData, int& memOffset);

private:
	int                               m_objectID = 0;
	std::vector<WorldObject>          m_objects;

};

#endif WORLDOBJECTMAP_H