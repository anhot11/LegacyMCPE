#pragma once

#include <format>
#include <vector>

#include "TileItem.h"

class ItemInstance;
class Tile;

class ColoredTileItem : public TileItem {
private:
    // Was const, but removing that so we don't have to make all the functions
    // const as well!
    Tile* colorTile;
    std::vector<int> descriptionPostfixes;

public:
    using TileItem::getColor;
    ColoredTileItem(int id, bool stackedByData);
    ~ColoredTileItem();

    virtual int getColor(std::shared_ptr<ItemInstance> item, int spriteLayer);
    virtual Icon* getIcon(int auxValue);
    virtual int getLevelDataForAuxValue(int auxValue);

    ColoredTileItem* setDescriptionPostfixes(
        std::vector<int>& descriptionPostfixes);
    virtual unsigned int getDescriptionId(
        std::shared_ptr<ItemInstance> instance);
};
