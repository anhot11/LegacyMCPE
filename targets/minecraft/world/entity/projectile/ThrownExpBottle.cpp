#include "ThrownExpBottle.h"

#include "java/JavaMath.h"
#include "java/Random.h"
#include "minecraft/world/entity/ExperienceOrb.h"
#include "minecraft/world/entity/projectile/Throwable.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/LevelEvent.h"

class LivingEntity;

ThrownExpBottle::ThrownExpBottle(Level* level) : Throwable(level) {}

ThrownExpBottle::ThrownExpBottle(Level* level,
                                 std::shared_ptr<LivingEntity> mob)
    : Throwable(level, mob) {}

ThrownExpBottle::ThrownExpBottle(Level* level, double x, double y, double z)
    : Throwable(level, x, y, z) {}

float ThrownExpBottle::getGravity() { return 0.07f; }

float ThrownExpBottle::getThrowPower() { return 0.7f; }

float ThrownExpBottle::getThrowUpAngleOffset() { return -20; }

void ThrownExpBottle::onHit(HitResult* res) {
    if (!level->isClientSide) {
        level->levelEvent(LevelEvent::PARTICLES_POTION_SPLASH,
                          (int)Math::round(x), (int)Math::round(y),
                          (int)Math::round(z), 0);

        int xpCount = 3 + level->random->nextInt(5) + level->random->nextInt(5);
        while (xpCount > 0) {
            int newCount = ExperienceOrb::getExperienceValue(xpCount);
            xpCount -= newCount;
            level->addEntity(std::shared_ptr<ExperienceOrb>(
                new ExperienceOrb(level, x, y, z, newCount)));
        }

        remove();
    }
}