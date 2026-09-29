#include "PickaxeItem.h"

#include <memory>
#include <vector>

#include "minecraft/world/item/DiggerItem.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/HalfSlabTile.h"
#include "minecraft/world/level/tile/Tile.h"

std::vector<Tile*> PickaxeItem::diggables;

void PickaxeItem::staticCtor() {
    PickaxeItem::diggables = std::vector<Tile*>(PICKAXE_DIGGABLES);
    diggables[0] = Tile::cobblestone;
    diggables[1] = Tile::stoneSlab;
    diggables[2] = Tile::stoneSlabHalf;
    diggables[3] = Tile::stone;
    diggables[4] = Tile::sandStone;
    diggables[5] = Tile::mossyCobblestone;
    diggables[6] = Tile::ironOre;
    diggables[7] = Tile::ironBlock;
    diggables[8] = Tile::coalOre;
    diggables[9] = Tile::goldBlock;
    diggables[10] = Tile::goldOre;
    diggables[11] = Tile::diamondOre;
    diggables[12] = Tile::diamondBlock;
    diggables[13] = Tile::ice;
    diggables[14] = Tile::netherRack;
    diggables[15] = Tile::lapisOre;
    diggables[16] = Tile::lapisBlock;
    diggables[17] = Tile::redStoneOre;
    diggables[18] = Tile::redStoneOre_lit;
    diggables[19] = Tile::rail;
    diggables[20] = Tile::detectorRail;
    diggables[21] = Tile::goldenRail;
    diggables[21] = Tile::activatorRail;
}

PickaxeItem::PickaxeItem(int id, const Tier* tier)
    : DiggerItem(id, 2, tier, &diggables) {}

bool PickaxeItem::canDestroySpecial(Tile* tile) {
    if (tile == Tile::obsidian) return tier->getLevel() == 3;
    if (tile == Tile::diamondBlock || tile == Tile::diamondOre)
        return tier->getLevel() >= 2;
    if (tile == Tile::emeraldBlock || tile == Tile::emeraldOre)
        return tier->getLevel() >= 2;
    if (tile == Tile::goldBlock || tile == Tile::goldOre)
        return tier->getLevel() >= 2;
    if (tile == Tile::ironBlock || tile == Tile::ironOre)
        return tier->getLevel() >= 1;
    if (tile == Tile::lapisBlock || tile == Tile::lapisOre)
        return tier->getLevel() >= 1;
    if (tile == Tile::redStoneOre || tile == Tile::redStoneOre_lit)
        return tier->getLevel() >= 2;
    if (tile->material == Material::stone) return true;
    if (tile->material == Material::metal) return true;
    if (tile->material == Material::heavyMetal) return true;
    return false;
}

// 4J - brought forward from 1.2.3
float PickaxeItem::getDestroySpeed(std::shared_ptr<ItemInstance> itemInstance,
                                   Tile* tile) {
    if (tile != nullptr && (tile->material == Material::metal ||
                            tile->material == Material::heavyMetal ||
                            tile->material == Material::stone)) {
        return speed;
    }
    return DiggerItem::getDestroySpeed(itemInstance, tile);
}
