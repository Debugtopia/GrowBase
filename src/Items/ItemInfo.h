#ifndef ITEMINFO_H
#define ITEMINFO_H
#include <cstdint>
#include <string>

#include <Items/Defs.h>

class ItemInfo
{
public:
    ItemInfo() = default;
    ~ItemInfo() = default;

    uint32_t ID = 0;
    uint16_t editableTypes = 0;
    uint8_t type = 0;
    uint8_t soundType = 0;
    std::string name = ""; // Cyphered on >= V3

    std::string texture = "";
    uint32_t textureHash = 0;

    uint8_t visualType = 0;
    uint32_t cookingTime = 0;
    int8_t textureX = 0;
    int8_t textureY = 0;

    uint8_t tileStorage = 0;
    uint8_t layer = 0;
    uint8_t tileCollision = 0;

    uint8_t hardness = 0;
    uint32_t regenTime = 0;
    uint8_t bodyPart = 0;
    uint16_t rarity = 999;
    uint8_t maxCount = 200;

    std::string textureExtra = "";
    uint32_t textureExtraHash = 0;
    uint32_t animMS = 400;

    std::string petName = ""; // V4
    std::string petPrefix = ""; // V4
    std::string petSuffix = ""; // V4
    std::string petAbility = ""; // V5

    uint8_t seedBase = 0;
    uint8_t seedOver = 0;
    uint8_t treeBase = 0;
    uint8_t treeOver = 0;
    uint32_t seedColor = 0;
    uint32_t treeColor = 0;
    uint16_t seed1 = 0;
    uint16_t seed2 = 0;
    uint32_t bloomTime = 31;

    uint32_t animationType = 0; // V7
    std::string animString = ""; // V7
    std::string animTexture = ""; // V8
    std::string animString2 = ""; // V8

    uint32_t DLayer1 = 0; // V8
    uint32_t DLayer2 = 0; // V8
    uint32_t flags = 0; // V9

    uint8_t clientData[60] = {}; // V9
    uint32_t tileRange = 0; // V10
    uint32_t pileRange = 0; // V10
    std::string customPunch = ""; // V11

    uint32_t fxFlags = 0; // V12
    uint8_t bodyParts[9] = {}; // V12

    uint32_t clockDivider = 0; // V13

    uint32_t parentID = 0; // V14

    uint8_t sitable = 0; // V15
    uint32_t sitOffsetX = 0; // V15
    uint32_t sitOffsetY = 0; // V15
    uint32_t sitOverlayX = 0; // V15
    uint32_t sitOverlayY = 0; // V15
    uint32_t sitOverlayOffsetX = 0; // V15
    uint32_t sitOverlayOffsetY = 0; // V15
    std::string sitTexture = ""; // V15

    std::string rendererFile = ""; // V16

    int unwantedV17 = 0; // V17

    uint32_t rendererHash = 0; // V18

    uint8_t unwantedV19[9] = {}; // V19

    /*server sideinfo*/
    bool bLocked = false; //item is locked, unable to be used by anyone but devs

    uint8_t punchID = 0; //punch effect
    uint8_t punchRangeModifier = 0; //range for punching
    uint8_t buildRangeModifier = 0; //range for placing

    uint8_t maxFruits = 4;
    uint32_t recycleValue = 0;
    uint8_t chi = ITEMELEMENT_NONE;
    std::string desc = "This item has no description.";
    uint16_t spliceOne = 0;
    uint16_t spliceTwo = 0;
    int playmodID = -1;
    uint8_t randGroup = 0; //rand seeds like tangram, growsabers
    uint32_t lockPower = 0;


    bool IsDropable();
    bool IsTrashable();
    bool IsSolidCollision();

    size_t GetMemoryEstimated(const uint16_t& version);
    void SerializeToMem(const uint16_t& version, uint8_t* pMem, int& offsetInOut);
    void SerializeFromMem(const uint16_t& version, uint8_t* pMem, int& offsetInOut);
};

#endif ITEMINFO_H