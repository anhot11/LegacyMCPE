#include "StoneButtonTile.h"

#include "minecraft/Facing.h"
#include "minecraft/world/level/tile/ButtonTile.h"
#include "minecraft/world/level/tile/Tile.h"

StoneButtonTile::StoneButtonTile(int id) : ButtonTile(id, false) {}

Icon* StoneButtonTile::getTexture(int face, int data) {
    return Tile::stone->getTexture(Facing::UP);
}