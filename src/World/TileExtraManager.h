#ifndef TILEEXTRAMANAGER_H
#define TILEEXTRAMANAGER_H
#include <string>


class TileExtraManager
{
public:
    TileExtraManager() = default;
    ~TileExtraManager() = default;

    // get
    bool                HasExtraData(const uint8_t& type, const uint16_t& worldMapVersion = 5);
    bool                IsSupported(const uint8_t& type, const uint16_t& worldMapVersion = 5);
};

TileExtraManager*       GetTileExtraManager();

#endif TILEEXTRAMANAGER_H