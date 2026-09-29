#include "ShovelItem.h"

#include <vector>

#include "minecraft/world/item/DiggerItem.h"
#include "minecraft/world/level/tile/GrassTile.h"
#include "minecraft/world/level/tile/MycelTile.h"
#include "minecraft/world/level/tile/Tile.h"

std::vector<Tile*>* ShovelItem::diggables = nullptr;

void ShovelItem::staticCtor() {
    ShovelItem::diggables = new std::vector<Tile*>(SHOVEL_DIGGABLES);
    (*diggables)[0] = Tile::grass;
    (*diggables)[1] = Tile::dirt;
    (*diggables)[2] = Tile::sand;
    (*diggables)[3] = Tile::gravel;
    (*diggables)[4] = Tile::topSnow;
    (*diggables)[5] = Tile::snow;
    (*diggables)[6] = Tile::clay;
    (*diggables)[7] = Tile::farmland;
    (*diggables)[8] = Tile::soulsand;
    (*diggables)[9] = Tile::mycel;
}

ShovelItem::ShovelItem(int id, const Tier* tier)
    : DiggerItem(id, 1, tier, diggables) {}

bool ShovelItem::canDestroySpecial(Tile* tile) {
    if (tile == Tile::topSnow) return true;
    if (tile == Tile::snow) return true;
    return false;
}