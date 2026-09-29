#pragma once

#include "TileItem.h"

class PistonTileItem : public TileItem {
public:
    PistonTileItem(int id);

    virtual int getLevelDataForAuxValue(int auxValue);
};