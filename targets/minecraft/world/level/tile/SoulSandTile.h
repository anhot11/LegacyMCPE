#pragma once
#include "Tile.h"

class SoulSandTile : public Tile {
public:
    SoulSandTile(int id);
    virtual std::optional<AABB> getAABB(Level* level, int x, int y, int z);
    virtual void entityInside(Level* level, int x, int y, int z,
                              std::shared_ptr<Entity> entity);
};
