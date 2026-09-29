#pragma once

#include <memory>

class TileEntity;
class Level;

class EntityTile {
public:
    virtual std::shared_ptr<TileEntity> newTileEntity(Level* level) = 0;
};