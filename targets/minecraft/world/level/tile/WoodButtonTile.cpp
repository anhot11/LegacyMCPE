#include "WoodButtonTile.h"

#include "minecraft/Facing.h"
#include "minecraft/world/level/tile/ButtonTile.h"
#include "minecraft/world/level/tile/Tile.h"

WoodButtonTile::WoodButtonTile(int id) : ButtonTile(id, true) {}

Icon* WoodButtonTile::getTexture(int face, int data) {
    return Tile::wood->getTexture(Facing::UP);
}