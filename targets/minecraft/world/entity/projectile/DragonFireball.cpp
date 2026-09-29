#include "DragonFireball.h"

#include <math.h>

#include <vector>

#include "java/JavaMath.h"
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/world/damageSource/DamageSource.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/projectile/Fireball.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/LevelEvent.h"
#include "minecraft/world/phys/AABB.h"
#include "minecraft/world/phys/HitResult.h"

class Entity;

const double DragonFireball::SPLASH_RANGE = 4.0;
const double DragonFireball::SPLASH_RANGE_SQ =
    DragonFireball::SPLASH_RANGE * DragonFireball::SPLASH_RANGE;

DragonFireball::DragonFireball(Level* level) : Fireball(level) {
    setSize(5 / 16.0f, 5 / 16.0f);
}

DragonFireball::DragonFireball(Level* level, std::shared_ptr<LivingEntity> mob,
                               double xa, double ya, double za)
    : Fireball(level, mob, xa, ya, za) {
    setSize(5 / 16.0f, 5 / 16.0f);
}

DragonFireball::DragonFireball(Level* level, double x, double y, double z,
                               double xa, double ya, double za)
    : Fireball(level, x, y, z, xa, ya, za) {
    setSize(5 / 16.0f, 5 / 16.0f);
}

void DragonFireball::onHit(HitResult* res) {
    if (!level->isClientSide) {
        AABB aoe = bb.grow(SPLASH_RANGE, SPLASH_RANGE / 2, SPLASH_RANGE);
        std::vector<std::shared_ptr<Entity> >* entitiesOfClass =
            level->getEntitiesOfClass(typeid(LivingEntity), &aoe);

        if (entitiesOfClass != nullptr && !entitiesOfClass->empty()) {
            // for (Entity e : entitiesOfClass)
            for (auto it = entitiesOfClass->begin();
                 it != entitiesOfClass->end(); ++it) {
                // shared_ptr<Entity> e = *it;
                std::shared_ptr<LivingEntity> e =
                    std::dynamic_pointer_cast<LivingEntity>(*it);
                double dist = distanceToSqr(e);
                if (dist < SPLASH_RANGE_SQ) {
                    double scale = 1.0 - (sqrt(dist) / SPLASH_RANGE);
                    if (e == res->entity) {
                        scale = 1;
                    }
                    e->hurt(DamageSource::dragonbreath, 8 * scale);
                }
            }
        }
        delete entitiesOfClass;
        level->levelEvent(LevelEvent::ENDERDRAGON_FIREBALL_SPLASH,
                          (int)Math::round(x), (int)Math::round(y),
                          (int)Math::round(z), 0);

        remove();
    }
}

bool DragonFireball::isPickable() { return false; }

bool DragonFireball::hurt(DamageSource* source, float damage) { return false; }

ePARTICLE_TYPE DragonFireball::getTrailParticleType() {
    return eParticleType_dragonbreath;
}

bool DragonFireball::shouldBurn() { return false; }
