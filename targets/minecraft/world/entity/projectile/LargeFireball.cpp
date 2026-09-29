#include "LargeFireball.h"

#include <string>

#include "minecraft/world/damageSource/DamageSource.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/projectile/Fireball.h"
#include "minecraft/world/level/GameRules.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/HitResult.h"
#include "nbt/CompoundTag.h"

LargeFireball::LargeFireball(Level* level) : Fireball(level) {
    explosionPower = 1;
}

LargeFireball::LargeFireball(Level* level, double x, double y, double z,
                             double xa, double ya, double za)
    : Fireball(level, x, y, z, xa, ya, za) {
    explosionPower = 1;
}

LargeFireball::LargeFireball(Level* level, std::shared_ptr<LivingEntity> mob,
                             double xa, double ya, double za)
    : Fireball(level, mob, xa, ya, za) {
    explosionPower = 1;
}

void LargeFireball::onHit(HitResult* res) {
    if (!level->isClientSide) {
        if (res->entity != nullptr) {
            DamageSource* damageSource = DamageSource::fireball(
                std::dynamic_pointer_cast<Fireball>(shared_from_this()), owner);
            res->entity->hurt(damageSource, 6);
            delete damageSource;
        }
        level->explode(
            nullptr, x, y, z, explosionPower, true,
            level->getGameRules()->getBoolean(GameRules::RULE_MOBGRIEFING));
        remove();
    }
}

void LargeFireball::addAdditonalSaveData(CompoundTag* tag) {
    Fireball::addAdditonalSaveData(tag);
    tag->putInt("ExplosionPower", explosionPower);
}

void LargeFireball::readAdditionalSaveData(CompoundTag* tag) {
    Fireball::readAdditionalSaveData(tag);
    if (tag->contains("ExplosionPower"))
        explosionPower = tag->getInt("ExplosionPower");
}
