#include <BaseApp.h> // precompiled

#include <Items/ItemInfo.h>
#include <SDK/Proton/MiscUtils.h>

bool ItemInfo::IsDropable()
{
    return (editableTypes & UNTRADEABLE) == false && ID != ITEM_ID_WORLD_KEY && ID != ITEM_ID_GUILD_KEY;
}

bool ItemInfo::IsTrashable()
{
    return ID != ITEM_ID_FIST && ID != ITEM_ID_WRENCH;
}

bool ItemInfo::IsSolidCollision()
{
    return tileCollision == TILE_COLLISION_FACTION ||
        tileCollision == TILE_COLLISION_GATEWAY ||
        tileCollision == TILE_COLLISION_SOLID ||
        tileCollision == TILE_COLLISION_VIP;
}

size_t ItemInfo::GetMemoryEstimated(const uint16_t& version)
{
    size_t len = 0;

    len += sizeof(uint32_t);
    len += sizeof(uint16_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);

    std::string itemName = name;
    if (version >= 3)
    {
        itemName = EncryptItemNameString(name, ID);
    }

    len += sizeof(uint16_t) + itemName.length();
    len += sizeof(uint16_t) + texture.length();
    len += sizeof(uint32_t);

    len += sizeof(uint8_t);
    len += sizeof(uint32_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);

    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);

    len += sizeof(uint8_t);
    len += sizeof(uint32_t);
    len += sizeof(uint8_t);
    len += sizeof(uint16_t);
    len += sizeof(uint8_t);

    len += sizeof(uint16_t) + textureExtra.length();
    len += sizeof(uint32_t);
    len += sizeof(uint32_t);

    if (version >= 4)
    {
        len += sizeof(uint16_t) + petName.length();
        len += sizeof(uint16_t) + petPrefix.length();
        len += sizeof(uint16_t) + petSuffix.length();
    }

    if (version >= 5)
    {
        len += sizeof(uint16_t) + petAbility.length();
    }

    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint8_t);
    len += sizeof(uint32_t);
    len += sizeof(uint32_t);
    len += sizeof(uint16_t);
    len += sizeof(uint16_t);
    len += sizeof(uint32_t);

    if (version >= 7)
    {
        len += sizeof(uint32_t);
        len += sizeof(uint16_t) + animString.length();
    }

    if (version >= 8)
    {
        len += sizeof(uint16_t) + animTexture.length();
        len += sizeof(uint16_t) + animString2.length();
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
    }

    if (version >= 9)
    {
        len += sizeof(uint32_t);
        len += 60; //unwantedV9
    }

    if (version >= 10)
    {
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
    }

    if (version >= 11)
    {
        len += sizeof(uint16_t) + customPunch.length();
    }

    if (version >= 12)
    {
        len += sizeof(uint32_t);
        len += 9; //bodyParts
    }

    if (version >= 13)
    {
        len += sizeof(uint32_t);
    }

    if (version >= 14)
    {
        len += sizeof(uint32_t);
    }

    if (version >= 15)
    {
        len += sizeof(uint8_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint32_t);
        len += sizeof(uint16_t) + sitTexture.length();
    }

    if (version >= 16)
    {
        len += sizeof(uint16_t) + rendererFile.length();
    }

    if (version >= 17)
    {
        len += sizeof(unwantedV17);
    }

    if (version >= 18)
    {
        len += sizeof(rendererHash);
    }

    if (version >= 19)
    {
        len += 9;
    }

    return len;
}

void ItemInfo::SerializeToMem(const uint16_t& version, uint8_t* pMem, int& offsetInOut)
{
    bool bWriteToMem = true;

    MemorySerializeRaw(ID, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(editableTypes, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(type, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(soundType, pMem, offsetInOut, bWriteToMem);

    std::string itemName = name;
    if (version >= 3)
    {
        itemName = EncryptItemNameString(name, ID);
    }

    MemorySerialize(itemName, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(texture, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureHash, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(visualType, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(cookingTime, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureX, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureY, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(tileStorage, pMem, offsetInOut, bWriteToMem);

    MemorySerializeRaw(layer, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(tileCollision, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(hardness, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(regenTime, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(bodyPart, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(rarity, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(maxCount, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(textureExtra, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureExtraHash, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(animMS, pMem, offsetInOut, bWriteToMem);

    if (version >= 4)
    {
        MemorySerialize(petName, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petPrefix, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petSuffix, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 5)
    {
        MemorySerialize(petAbility, pMem, offsetInOut, bWriteToMem);
    }

    MemorySerializeRaw(seedBase, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seedOver, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(treeBase, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(treeOver, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seedColor, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(treeColor, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seed1, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seed2, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(bloomTime, pMem, offsetInOut, bWriteToMem);

    if (version >= 7)
    {
        MemorySerializeRaw(animationType, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 8)
    {
        MemorySerialize(animTexture, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString2, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(DLayer1, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(DLayer2, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 9)
    {
        MemorySerializeRaw(flags, pMem, offsetInOut, bWriteToMem);
        for (int i = 0; i < 60; i++)
        {
            MemorySerializeRaw(clientData[i], pMem, offsetInOut, bWriteToMem);
        }
    }

    if (version >= 10)
    {
        MemorySerializeRaw(tileRange, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(pileRange, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 11)
    {
        MemorySerialize(customPunch, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 12)
    {
        MemorySerializeRaw(fxFlags, pMem, offsetInOut, bWriteToMem);
        for (int i = 0; i < 9; i++)
        {
            MemorySerializeRaw(bodyParts[i], pMem, offsetInOut, bWriteToMem);
        }
    }

    if (version >= 13)
    {
        MemorySerializeRaw(clockDivider, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 14)
    {
        MemorySerializeRaw(parentID, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 13)
    {
        MemorySerializeRaw(sitable, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayX, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayY, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(sitTexture, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 16)
    {
        MemorySerialize(rendererFile, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 17)
    {
        MemorySerializeRaw(unwantedV17, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 18)
    {
        MemorySerializeRaw(rendererHash, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 19)
    {
        for (int i = 0; i < 9; i++)
        {
            MemorySerializeRaw(unwantedV19[i], pMem, offsetInOut, bWriteToMem);
        }
    }
}

void ItemInfo::SerializeFromMem(const uint16_t& version, uint8_t* pMem, int& offsetInOut)
{
    bool bWriteToMem = false;

    MemorySerializeRaw(ID, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(editableTypes, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(type, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(soundType, pMem, offsetInOut, bWriteToMem);

    if (version >= 3)
    {
        MemorySerializeStringEncrypted(name, pMem, offsetInOut, bWriteToMem, ID, "PBG892FXX982ABC*");
    }
    else
    {
        MemorySerialize(name, pMem, offsetInOut, bWriteToMem);
    }

    MemorySerialize(texture, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureHash, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(visualType, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(cookingTime, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureX, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureY, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(tileStorage, pMem, offsetInOut, bWriteToMem);

    MemorySerializeRaw(layer, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(tileCollision, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(hardness, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(regenTime, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(bodyPart, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(rarity, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(maxCount, pMem, offsetInOut, bWriteToMem);
    MemorySerialize(textureExtra, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(textureExtraHash, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(animMS, pMem, offsetInOut, bWriteToMem);

    if (version >= 4)
    {
        MemorySerialize(petName, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petPrefix, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(petSuffix, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 5)
    {
        MemorySerialize(petAbility, pMem, offsetInOut, bWriteToMem);
    }

    MemorySerializeRaw(seedBase, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seedOver, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(treeBase, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(treeOver, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seedColor, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(treeColor, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seed1, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(seed2, pMem, offsetInOut, bWriteToMem);
    MemorySerializeRaw(bloomTime, pMem, offsetInOut, bWriteToMem);

    if (version >= 7)
    {
        MemorySerializeRaw(animationType, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 8)
    {
        MemorySerialize(animTexture, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(animString2, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(DLayer1, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(DLayer2, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 9)
    {
        MemorySerializeRaw(flags, pMem, offsetInOut, bWriteToMem);
        for (int i = 0; i < 60; i++)
        {
            MemorySerializeRaw(clientData[i], pMem, offsetInOut, bWriteToMem);
        }

    }

    if (version >= 10)
    {
        MemorySerializeRaw(tileRange, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(pileRange, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 11)
    {
        MemorySerialize(customPunch, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 12)
    {
        MemorySerializeRaw(fxFlags, pMem, offsetInOut, bWriteToMem);
        for (int i = 0; i < 9; i++)
        {
            MemorySerializeRaw(bodyParts[i], pMem, offsetInOut, bWriteToMem);
        }
    }

    if (version >= 13)
    {
        MemorySerializeRaw(clockDivider, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 14)
    {
        MemorySerializeRaw(parentID, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 13)
    {
        MemorySerializeRaw(sitable, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayX, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayY, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayOffsetX, pMem, offsetInOut, bWriteToMem);
        MemorySerializeRaw(sitOverlayOffsetY, pMem, offsetInOut, bWriteToMem);
        MemorySerialize(sitTexture, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 16)
    {
        MemorySerialize(rendererFile, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 17)
    {
        MemorySerializeRaw(unwantedV17, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 18)
    {
        MemorySerializeRaw(rendererHash, pMem, offsetInOut, bWriteToMem);
    }

    if (version >= 19)
    {
        for (int i = 0; i < 9; i++)
        {
            MemorySerializeRaw(unwantedV19[i], pMem, offsetInOut, bWriteToMem);
        }
    }
}