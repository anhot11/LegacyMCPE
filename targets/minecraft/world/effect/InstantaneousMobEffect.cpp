
#include "minecraft/world/effect/InstantaneousMobEffect.h"

#include "minecraft/GameEnums.h"
#include "minecraft/world/effect/MobEffect.h"

InstantenousMobEffect::InstantenousMobEffect(int id, bool isHarmful,
                                             eMinecraftColour color)
    : MobEffect(id, isHarmful, color) {}

bool InstantenousMobEffect::isInstantenous() { return true; }

bool InstantenousMobEffect::isDurationEffectTick(int remainingDuration,
                                                 int amplification) {
    return remainingDuration >= 1;
}