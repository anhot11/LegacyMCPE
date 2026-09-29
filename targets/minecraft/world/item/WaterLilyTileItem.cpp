#include "WaterLilyTileItem.h"

#include <memory>

#include "minecraft/world/entity/player/Abilities.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/ColoredTileItem.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/HitResult.h"

WaterLilyTileItem::WaterLilyTileItem(int id) : ColoredTileItem(id, false) {}

bool WaterLilyTileItem::TestUse(std::shared_ptr<ItemInstance> itemInstance,
                                Level* level, std::shared_ptr<Player> player) {
    HitResult* hr = getPlayerPOVHitResult(level, player, true);
    if (hr == nullptr) return false;

    if (hr->type == HitResult::TILE) {
        int xt = hr->x;
        int yt = hr->y;
        int zt = hr->z;
        if (!level->mayInteract(player, xt, yt, zt, 0)) {
            delete hr;
            return false;
        }
        if (!player->mayUseItemAt(xt, yt, zt, hr->f, itemInstance)) {
            delete hr;
            return false;
        }

        delete hr;
        if (level->getMaterial(xt, yt, zt) == Material::water &&
            level->getData(xt, yt, zt) == 0 &&
            level->isEmptyTile(xt, yt + 1, zt)) {
            return true;
        }
    } else {
        delete hr;
    }
    return false;
}

std::shared_ptr<ItemInstance> WaterLilyTileItem::use(
    std::shared_ptr<ItemInstance> itemInstance, Level* level,
    std::shared_ptr<Player> player) {
    HitResult* hr = getPlayerPOVHitResult(level, player, true);
    if (hr == nullptr) return itemInstance;

    if (hr->type == HitResult::TILE) {
        int xt = hr->x;
        int yt = hr->y;
        int zt = hr->z;
        if (!level->mayInteract(player, xt, yt, zt, 0)) {
            delete hr;
            return itemInstance;
        }
        if (!player->mayUseItemAt(xt, yt, zt, hr->f, itemInstance)) {
            delete hr;
            return itemInstance;
        }

        delete hr;
        if (level->getMaterial(xt, yt, zt) == Material::water &&
            level->getData(xt, yt, zt) == 0 &&
            level->isEmptyTile(xt, yt + 1, zt)) {
            level->setTileAndUpdate(xt, yt + 1, zt, Tile::waterLily->id);
            if (!player->abilities.instabuild) {
                itemInstance->count--;
            }
        }
    } else {
        delete hr;
    }
    return itemInstance;
}

int WaterLilyTileItem::getColor(int data, int spriteLayer) {
    return Tile::waterLily->getColor(data);
}