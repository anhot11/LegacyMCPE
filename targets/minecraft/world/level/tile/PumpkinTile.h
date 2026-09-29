#pragma once
#include <string>

#include "DirectionalTile.h"

class Mob;
class ChunkRebuildData;
class Icon;

class PumpkinTile : public DirectionalTile {
    friend class Tile;
    friend class ChunkRebuildData;

public:
    static const int DIR_SOUTH = 0;
    static const int DIR_WEST = 1;
    static const int DIR_NORTH = 2;
    static const int DIR_EAST = 3;

private:
    static const std::string TEXTURE_FACE;
    static const std::string TEXTURE_LANTERN;
    bool lit;
    Icon* iconTop;
    Icon* iconFace;

protected:
    PumpkinTile(int id, bool lit);

public:
    virtual Icon* getTexture(int face, int data);
    virtual void onPlace(Level* level, int x, int y, int z);
    virtual bool mayPlace(Level* level, int x, int y, int z);
    virtual void setPlacedBy(Level* level, int x, int y, int z,
                             std::shared_ptr<LivingEntity> by,
                             std::shared_ptr<ItemInstance> itemInstance);
    void registerIcons(IconRegister* iconRegister);
};