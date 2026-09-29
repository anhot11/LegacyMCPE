#include "HealthBoostMobEffect.h"

#include <memory>

#include "minecraft/world/effect/MobEffect.h"
#include "minecraft/world/entity/LivingEntity.h"

HealthBoostMobEffect::HealthBoostMobEffect(int id, bool isHarmful,
                                           eMinecraftColour color)
    : MobEffect(id, isHarmful, color) {}

void HealthBoostMobEffect::removeAttributeModifiers(
    std::shared_ptr<LivingEntity> entity, BaseAttributeMap* attributes,
    int amplifier) {
    MobEffect::removeAttributeModifiers(entity, attributes, amplifier);
    if (entity->getHealth() > entity->getMaxHealth()) {
        entity->setHealth(entity->getMaxHealth());
    }
}
