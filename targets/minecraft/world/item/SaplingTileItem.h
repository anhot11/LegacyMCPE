#pragma once

#include "TileItem.h"

class SaplingTileItem : public TileItem {
public:
    SaplingTileItem(int id);

    virtual int getLevelDataForAuxValue(int auxValue);
    virtual Icon* getIcon(int itemAuxValue);

    // 4J brought forward to have unique names for different sapling types
    virtual unsigned int getDescriptionId(
        std::shared_ptr<ItemInstance> instance);
};