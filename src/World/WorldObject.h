
#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H
#include <cstdint>

enum eObjectChangeTypes
{
	CHANGETYPE_EDIT = -3, // modify / edit the object, it's itemID, flags or count
	CHANGETYPE_REMOVE, // -2 - removes the object on the client side entirely
	CHANGETYPE_SPAWN, // -1 - spawns new object to the client side
	CHANGETYPE_PICKUP // 0 - used when a player picks the dropped object up
};

//eWorldObjectFlags
#define WORLDOBJECT_FLAGS_NONE = 0x0
#define WORLDOBJECT_FLAG_SHRINK 0x0001 // adds animation, where when object is picked up, it shrinks instead of going to the inventory
#define WORLDOBJECT_FLAG_UNKNOWN_2 0x0002
#define WORLDOBJECT_FLAG_UNPICKED_FOSSIL 0x0004 // set when using fossil chisel to extract Fossil from a Fossil Rock, allows Fossil Brush to be used on the dropped fossil
#define WORLDOBJECT_FLAG_RADIOACTIVE 0x0008 // adds radioactive animation, used on star dusts when they're spawned by the comet day event itself
#define WORLDOBJECT_FLAG_UNKNOWN_16 0x0010
#define WORLDOBJECT_FLAG_UNKNOWN_32 0x0020
#define WORLDOBJECT_FLAG_UNKNOWN_64 0x0040
#define WORLDOBJECT_FLAG_UNKNOWN_128 0x0080

class WorldObject 
{
public:
	WorldObject() = default;
	~WorldObject() = default;


    // get
	size_t            GetMemoryEstimated();


    // fn
	void              Serialize(uint8_t * pData, int& memOffset);
	void              Load(uint8_t * pMem, int& memOffset);

public:
	short             itemID = 0;
	float             x = 0.f;
	float             y = 0.f;
	uint8_t           count = 0;
	uint8_t           flags = 0;
	int               ID = 0;
};

#endif WORLDOBJECT_H