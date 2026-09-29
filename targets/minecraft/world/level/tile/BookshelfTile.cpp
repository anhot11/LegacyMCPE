#include "BookshelfTile.h"

#include "minecraft/Facing.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

BookshelfTile::BookshelfTile(int id) : Tile(id, Material::wood) {}

Icon* BookshelfTile::getTexture(int face, int data) {
    if (face == Facing::UP || face == Facing::DOWN)
        return Tile::wood->getTexture(face);
    return Tile::getTexture(face, data);
}

int BookshelfTile::getResourceCount(Random* random) { return 3; }

int BookshelfTile::getResource(int data, Random* random, int playerBonusLevel) {
    return Item::book_Id;
}