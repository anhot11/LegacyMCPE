#include "ShovelItem.h"

#include <vector>

#include "minecraft/world/item/DiggerItem.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/Level.h"
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

bool ShovelItem::useOn(std::shared_ptr<ItemInstance> instance,
                       std::shared_ptr<Player> player, Level* level, int x,
                       int y, int z, int face, float clickX, float clickY,
                       float clickZ, bool bTestUseOnOnly) {
    if (!player->mayUseItemAt(x, y, z, face, instance)) return false;

    int targetTile = level->getTile(x, y, z);
    int aboveTile = level->getTile(x, y + 1, z);

    // Right-clicking / tapping grass with shovel creates Grass Path (MCPE 0.15 feature)
    if (face != 0 && aboveTile == 0 && targetTile == Tile::grass_Id) {
        if (Tile::grassPath != nullptr) {
            if (bTestUseOnOnly) return true;
            Tile* tile = Tile::grassPath;
            level->playSound(x + 0.5f, y + 0.5f, z + 0.5f,
                             tile->soundType->getStepSound(),
                             (tile->soundType->getVolume() + 1.0f) / 2.0f,
                             tile->soundType->getPitch() * 0.8f);
            if (level->isClientSide) return true;
            level->setTileAndUpdate(x, y, z, tile->id);
            instance->hurtAndBreak(1, player);
            return true;
        }
    }
    return false;
}