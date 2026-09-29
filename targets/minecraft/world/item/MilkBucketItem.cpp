#include "MilkBucketItem.h"

#include <memory>

#include "minecraft/world/entity/player/Abilities.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"

MilkBucketItem::MilkBucketItem(int id) : Item(id) { setMaxStackSize(1); }

std::shared_ptr<ItemInstance> MilkBucketItem::useTimeDepleted(
    std::shared_ptr<ItemInstance> instance, Level* level,
    std::shared_ptr<Player> player) {
    if (!player->abilities.instabuild) instance->count--;

    if (!level->isClientSide) {
        player->removeAllEffects();
    }

    if (instance->count <= 0) {
        return std::shared_ptr<ItemInstance>(
            new ItemInstance(Item::bucket_empty));
    }
    return instance;
}

int MilkBucketItem::getUseDuration(std::shared_ptr<ItemInstance> itemInstance) {
    return DRINK_DURATION;
}

UseAnim MilkBucketItem::getUseAnimation(
    std::shared_ptr<ItemInstance> itemInstance) {
    return UseAnim_drink;
}

std::shared_ptr<ItemInstance> MilkBucketItem::use(
    std::shared_ptr<ItemInstance> instance, Level* level,
    std::shared_ptr<Player> player) {
    player->startUsingItem(instance, getUseDuration(instance));
    return instance;
}