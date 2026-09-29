#include "HoeItem.h"

#include <memory>

#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"

HoeItem::HoeItem(int id, const Tier* tier) : Item(id) {
    this->tier = tier;
    maxStackSize = 1;
    setMaxDamage(tier->getUses());
}

bool HoeItem::useOn(std::shared_ptr<ItemInstance> instance,
                    std::shared_ptr<Player> player, Level* level, int x, int y,
                    int z, int face, float clickX, float clickY, float clickZ,
                    bool bTestUseOnOnly) {
    if (!player->mayUseItemAt(x, y, z, face, instance)) return false;

    // 4J-PB - Adding a test only version to allow tooltips to be displayed

    int targetType = level->getTile(x, y, z);
    int above = level->getTile(x, y + 1, z);

    if (face != 0 && above == 0 &&
        (targetType == Tile::grass_Id || targetType == Tile::dirt_Id)) {
        if (!bTestUseOnOnly) {
            Tile* tile = Tile::farmland;
            level->playSound(x + 0.5f, y + 0.5f, z + 0.5f,
                             tile->soundType->getStepSound(),
                             (tile->soundType->getVolume() + 1) / 2,
                             tile->soundType->getPitch() * 0.8f);

            if (level->isClientSide) return true;
            level->setTileAndUpdate(x, y, z, tile->id);
            instance->hurtAndBreak(1, player);
        }
        return true;
    }

    return false;
}

bool HoeItem::isHandEquipped() { return true; }

const Item::Tier* HoeItem::getTier() { return tier; }
