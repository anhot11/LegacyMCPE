#include "RedlightTile.h"

#include <string>

#include "minecraft/world/IconRegister.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

RedlightTile::RedlightTile(int id, bool isLit)
    : Tile(id, Material::buildable_glass) {
    this->isLit = isLit;

    if (isLit) {
        setLightEmission(1.0f);
    }
}

void RedlightTile::registerIcons(IconRegister* iconRegister) {
    if (isLit) {
        icon = iconRegister->registerIcon("redstoneLight_lit");
    } else {
        icon = iconRegister->registerIcon("redstoneLight");
    }
}

void RedlightTile::onPlace(Level* level, int x, int y, int z) {
    if (!level->isClientSide) {
        if (isLit && !level->hasNeighborSignal(x, y, z)) {
            level->addToTickNextTick(x, y, z, id, 4);
        } else if (!isLit && level->hasNeighborSignal(x, y, z)) {
            level->setTileAndData(x, y, z, Tile::redstoneLight_lit_Id, 0,
                                  UPDATE_CLIENTS);
        }
    }
}

void RedlightTile::neighborChanged(Level* level, int x, int y, int z,
                                   int type) {
    if (!level->isClientSide) {
        if (isLit && !level->hasNeighborSignal(x, y, z)) {
            level->addToTickNextTick(x, y, z, id, 4);
        } else if (!isLit && level->hasNeighborSignal(x, y, z)) {
            level->setTileAndData(x, y, z, Tile::redstoneLight_lit_Id, 0,
                                  UPDATE_CLIENTS);
        }
    }
}

void RedlightTile::tick(Level* level, int x, int y, int z, Random* random) {
    if (!level->isClientSide) {
        if (isLit && !level->hasNeighborSignal(x, y, z)) {
            level->setTileAndData(x, y, z, Tile::redstoneLight_Id, 0,
                                  UPDATE_CLIENTS);
        }
    }
}

int RedlightTile::getResource(int data, Random* random, int playerBonusLevel) {
    return Tile::redstoneLight_Id;
}

int RedlightTile::cloneTileId(Level* level, int x, int y, int z) {
    return Tile::redstoneLight_Id;
}