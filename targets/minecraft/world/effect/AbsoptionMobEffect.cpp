#include "AbsoptionMobEffect.h"

#include <memory>

#include "minecraft/world/effect/MobEffect.h"
#include "minecraft/world/entity/LivingEntity.h"

AbsoptionMobEffect::AbsoptionMobEffect(int id, bool isHarmful,
                                       eMinecraftColour color)
    : MobEffect(id, isHarmful, color) {}

void AbsoptionMobEffect::removeAttributeModifiers(
    std::shared_ptr<LivingEntity> entity, BaseAttributeMap* attributes,
    int amplifier) {
    entity->setAbsorptionAmount(entity->getAbsorptionAmount() -
                                4 * (amplifier + 1));
    MobEffect::removeAttributeModifiers(entity, attributes, amplifier);
}

void AbsoptionMobEffect::addAttributeModifiers(
    std::shared_ptr<LivingEntity> entity, BaseAttributeMap* attributes,
    int amplifier) {
    entity->setAbsorptionAmount(entity->getAbsorptionAmount() +
                                4 * (amplifier + 1));
    MobEffect::addAttributeModifiers(entity, attributes, amplifier);
}
