#pragma once

#include "AuxDataTileItem.h"

class Tile;

class SnowItem : public AuxDataTileItem {
public:
    SnowItem(int id, Tile* parentTile);

    bool useOn(std::shared_ptr<ItemInstance> instance,
               std::shared_ptr<Player> player, Level* level, int x, int y,
               int z, int face, float clickX, float clickY, float clickZ,
               bool bTestUseOnOnly = false);
};