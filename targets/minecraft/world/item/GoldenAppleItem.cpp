#include "GoldenAppleItem.h"

#include <memory>

#include "minecraft/SharedConstants.h"
#include "minecraft/world/effect/MobEffect.h"
#include "minecraft/world/effect/MobEffectInstance.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/FoodItem.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/Rarity.h"
#include "minecraft/world/level/Level.h"
#include "strings.h"

GoldenAppleItem::GoldenAppleItem(int id, int nutrition, float saturationMod,
                                 bool isMeat)
    : FoodItem(id, nutrition, saturationMod, isMeat) {
    setStackedByData(true);
}

bool GoldenAppleItem::isFoil(std::shared_ptr<ItemInstance> itemInstance) {
    return itemInstance->getAuxValue() > 0;
}

const Rarity* GoldenAppleItem::getRarity(
    std::shared_ptr<ItemInstance> itemInstance) {
    if (itemInstance->getAuxValue() == 0) {
        return Rarity::rare;
    }
    return Rarity::epic;
}

void GoldenAppleItem::addEatEffect(std::shared_ptr<ItemInstance> instance,
                                   Level* level,
                                   std::shared_ptr<Player> player) {
    if (!level->isClientSide)
        player->addEffect(new MobEffectInstance(
            MobEffect::absorption->id,
            2 * 60 * SharedConstants::TICKS_PER_SECOND, 0));

    if (instance->getAuxValue() > 0) {
        if (!level->isClientSide) {
            player->addEffect(new MobEffectInstance(
                MobEffect::regeneration->id,
                30 * SharedConstants::TICKS_PER_SECOND, 4));
            player->addEffect(new MobEffectInstance(
                MobEffect::damageResistance->id,
                300 * SharedConstants::TICKS_PER_SECOND, 0));
            player->addEffect(new MobEffectInstance(
                MobEffect::fireResistance->id,
                300 * SharedConstants::TICKS_PER_SECOND, 0));
        }
    } else {
        FoodItem::addEatEffect(instance, level, player);
    }
}

unsigned int GoldenAppleItem::getUseDescriptionId(int iData /*= -1*/) {
    if (iData == 0)
        return IDS_DESC_GOLDENAPPLE;
    else
        return IDS_DESC_ENCHANTED_GOLDENAPPLE;
}

unsigned int GoldenAppleItem::getUseDescriptionId(
    std::shared_ptr<ItemInstance> instance) {
    return this->getUseDescriptionId(instance->getAuxValue());
}