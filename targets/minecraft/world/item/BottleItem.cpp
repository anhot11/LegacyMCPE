#include "BottleItem.h"

#include <memory>

#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/PotionItem.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/phys/HitResult.h"

BottleItem::BottleItem(int id) : Item(id) {}

Icon* BottleItem::getIcon(int auxValue) { return Item::potion->getIcon(0); }

std::shared_ptr<ItemInstance> BottleItem::use(
    std::shared_ptr<ItemInstance> itemInstance, Level* level,
    std::shared_ptr<Player> player) {
    HitResult* hr = getPlayerPOVHitResult(level, player, true);
    if (hr == nullptr) return itemInstance;

    if (hr->type == HitResult::TILE) {
        int xt = hr->x;
        int yt = hr->y;
        int zt = hr->z;
        delete hr;

        if (!level->mayInteract(player, xt, yt, zt, 0)) {
            return itemInstance;
        }
        if (!player->mayUseItemAt(xt, yt, zt, hr->f, itemInstance)) {
            return itemInstance;
        }
        if (level->getMaterial(xt, yt, zt) == Material::water) {
            itemInstance->count--;
            if (itemInstance->count <= 0) {
                return std::shared_ptr<ItemInstance>(
                    new ItemInstance((Item*)Item::potion));
            } else {
                if (!player->inventory->add(std::shared_ptr<ItemInstance>(
                        new ItemInstance((Item*)Item::potion)))) {
                    player->drop(std::shared_ptr<ItemInstance>(
                        new ItemInstance(Item::potion_Id, 1, 0)));
                }
            }
        }
    } else {
        delete hr;
    }

    return itemInstance;
}

// 4J-PB - added to allow tooltips
bool BottleItem::TestUse(std::shared_ptr<ItemInstance> itemInstance,
                         Level* level, std::shared_ptr<Player> player) {
    HitResult* hr = getPlayerPOVHitResult(level, player, true);
    if (hr == nullptr) return false;

    if (hr->type == HitResult::TILE) {
        int xt = hr->x;
        int yt = hr->y;
        int zt = hr->z;
        delete hr;

        if (!level->mayInteract(player, xt, yt, zt, 0)) {
            return false;
        }
        if (!player->mayUseItemAt(xt, yt, zt, hr->f, itemInstance)) {
            return false;
        }
        if (level->getMaterial(xt, yt, zt) == Material::water) {
            return true;
        }
    } else {
        delete hr;
    }

    return false;
}

void BottleItem::registerIcons(IconRegister* iconRegister) {
    // We reuse another texture.
}