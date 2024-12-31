#include <BaseApp.h> // precompiled
#include <World/TileExtraManager.h>

#include <Items/Defs.h> // for enums

TileExtraManager g_tileExtraManager;
TileExtraManager * GetTileExtraManager() { return &g_tileExtraManager; }

bool TileExtraManager::HasExtraData(const uint8_t& type, const uint16_t& worldMapVersion)
{
    switch (type)
    {
        case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
        {
            // door tile extra type
            // tile extra type: 1
            return true;
        }
    }

    return false;
}

bool TileExtraManager::IsSupported(const uint8_t& type, const uint16_t& worldMapVersion)
{
    switch (type)
    {
        case TYPE_DOOR: case TYPE_PORTAL: case TYPE_MAIN_DOOR:
        {
            // door tile extra type
            // tile extra type: 1
            return true;
        }
    }

    return false;
}