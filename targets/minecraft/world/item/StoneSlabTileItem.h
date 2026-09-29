#pragma once

#include <memory>

#include "TileItem.h"
#include "minecraft/world/level/tile/HalfSlabTile.h"

class HalfSlabTile;
class ItemInstance;
class Level;
class Player;

class StoneSlabTileItem : public TileItem {
private:
    bool isFull;
    HalfSlabTile* halfTile;
    HalfSlabTile* fullTile;

public:
    StoneSlabTileItem(int id, HalfSlabTile* halfTile, HalfSlabTile* fullTile,
                      bool full);

    virtual Icon* getIcon(int itemAuxValue);
    virtual int getLevelDataForAuxValue(int auxValue);
    virtual unsigned int getDescriptionId(
        std::shared_ptr<ItemInstance> instance);
    virtual bool useOn(std::shared_ptr<ItemInstance> instance,
                       std::shared_ptr<Player> player, Level* level, int x,
                       int y, int z, int face, float clickX, float clickY,
                       float clickZ, bool bTestUseOnOnly = false);

    virtual bool mayPlace(Level* level, int x, int y, int z, int face,
                          std::shared_ptr<Player> player,
                          std::shared_ptr<ItemInstance> item);

private:
    bool tryConvertTargetTile(std::shared_ptr<ItemInstance> instance,
                              std::shared_ptr<Player> player, Level* level,
                              int x, int y, int z, int face,
                              bool bTestUseOnOnly);
};