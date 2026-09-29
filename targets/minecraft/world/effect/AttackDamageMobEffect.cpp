#include "AttackDamageMobEffect.h"

#include "minecraft/world/effect/MobEffect.h"

AttackDamageMobEffect::AttackDamageMobEffect(int id, bool isHarmful,
                                             eMinecraftColour color)
    : MobEffect(id, isHarmful, color) {}

double AttackDamageMobEffect::getAttributeModifierValue(
    int amplifier, AttributeModifier* original) {
    if (id == MobEffect::weakness->id) {
        return -0.5f * (amplifier + 1);
    } else {
        return 1.3 * (amplifier + 1);
    }
}