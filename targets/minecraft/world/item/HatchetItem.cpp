#include "HatchetItem.h"

#include <memory>
#include <vector>

#include "minecraft/world/item/DiggerItem.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/ChestTile.h"
#include "minecraft/world/level/tile/HalfSlabTile.h"
#include "minecraft/world/level/tile/Tile.h"

std::vector<Tile*>* HatchetItem::diggables = nullptr;

void HatchetItem::staticCtor() {
    HatchetItem::diggables = new std::vector<Tile*>(HATCHET_DIGGABLES);
    (*diggables)[0] = Tile::wood;
    (*diggables)[1] = Tile::bookshelf;
    (*diggables)[2] = Tile::treeTrunk;
    (*diggables)[3] = Tile::chest;
    // 4J - brought forward from 1.2.3
    (*diggables)[4] = Tile::stoneSlab;
    (*diggables)[5] = Tile::stoneSlabHalf;
    (*diggables)[6] = Tile::pumpkin;
    (*diggables)[7] = Tile::litPumpkin;
}

HatchetItem::HatchetItem(int id, const Tier* tier)
    : DiggerItem(id, 3, tier, diggables) {}

// 4J - brought forward from 1.2.3
float HatchetItem::getDestroySpeed(std::shared_ptr<ItemInstance> itemInstance,
                                   Tile* tile) {
    if (tile != nullptr && (tile->material == Material::wood ||
                            tile->material == Material::plant ||
                            tile->material == Material::replaceable_plant)) {
        return speed;
    }
    return DiggerItem::getDestroySpeed(itemInstance, tile);
}