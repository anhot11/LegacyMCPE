#include "BreedGoal.h"

#include <limits>
#include <vector>

#include "java/Class.h"
#include "java/Random.h"
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/stats/GenericStats.h"
#include "minecraft/world/entity/AgeableMob.h"
#include "minecraft/world/entity/ExperienceOrb.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/animal/Animal.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/AABB.h"

class Entity;

BreedGoal::BreedGoal(Animal* animal, double speedModifier) {
    partner = std::weak_ptr<Animal>();
    loveTime = 0;

    this->animal = animal;
    this->level = animal->level;
    this->speedModifier = speedModifier;
    setRequiredControlFlags(Control::MoveControlFlag |
                            Control::LookControlFlag);
}

bool BreedGoal::canUse() {
    if (!animal->isInLove()) return false;
    partner = std::weak_ptr<Animal>(getFreePartner());
    return partner.lock() != nullptr;
}

bool BreedGoal::canContinueToUse() {
    return partner.lock() != nullptr && partner.lock()->isAlive() &&
           partner.lock()->isInLove() && loveTime < 20 * 3;
}

void BreedGoal::stop() {
    partner = std::weak_ptr<Animal>();
    loveTime = 0;
}

void BreedGoal::tick() {
    animal->getLookControl()->setLookAt(partner.lock(), 10,
                                        animal->getMaxHeadXRot());
    animal->getNavigation()->moveTo(partner.lock(), speedModifier);
    ++loveTime;
    if (loveTime >= 20 * 3 && animal->distanceToSqr(partner.lock()) < 3 * 3)
        breed();
}

std::shared_ptr<Animal> BreedGoal::getFreePartner() {
    float r = 8;
    AABB grown_bb = animal->bb.grow(r, r, r);
    std::vector<std::shared_ptr<Entity> >* others =
        level->getEntitiesOfClass(typeid(*animal), &grown_bb);
    double dist = std::numeric_limits<double>::max();
    std::shared_ptr<Animal> partner = nullptr;
    for (auto it = others->begin(); it != others->end(); ++it) {
        std::shared_ptr<Animal> p = std::dynamic_pointer_cast<Animal>(*it);
        if (animal->canMate(p) && animal->distanceToSqr(p) < dist) {
            partner = p;
            dist = animal->distanceToSqr(p);
        }
    }
    delete others;
    return partner;
}

void BreedGoal::breed() {
    std::shared_ptr<AgableMob> offspring =
        animal->getBreedOffspring(partner.lock());
    animal->setDespawnProtected();
    partner.lock()->setDespawnProtected();
    if (offspring == nullptr) {
        // This will be nullptr if we've hit our limits for spawning any
        // particular type of animal... reset things as normally as we can,
        // without actually producing any offspring
        animal->resetLove();
        partner.lock()->resetLove();
        return;
    }

    std::shared_ptr<Player> loveCause = animal->getLoveCause();
    if (loveCause == nullptr && partner.lock()->getLoveCause() != nullptr) {
        loveCause = partner.lock()->getLoveCause();
    }

    if (loveCause != nullptr) {
        // Record mob bred stat.
        loveCause->awardStat(
            GenericStats::breedEntity(offspring->GetType()),
            GenericStats::param_breedEntity(offspring->GetType()));

        if (animal->GetType() == eTYPE_COW) {
            // loveCause->awardStat(Achievements.breedCow);
        }
    }

    animal->setAge(5 * 60 * 20);
    partner.lock()->setAge(5 * 60 * 20);
    animal->resetLove();
    partner.lock()->resetLove();
    offspring->setAge(AgableMob::BABY_START_AGE);
    offspring->moveTo(animal->x, animal->y, animal->z, 0, 0);
    offspring->setDespawnProtected();
    level->addEntity(offspring);

    Random* random = animal->getRandom();
    for (int i = 0; i < 7; i++) {
        double xa = random->nextGaussian() * 0.02;
        double ya = random->nextGaussian() * 0.02;
        double za = random->nextGaussian() * 0.02;
        level->addParticle(
            eParticleType_heart,
            animal->x + random->nextFloat() * animal->bbWidth * 2 -
                animal->bbWidth,
            animal->y + .5f + random->nextFloat() * animal->bbHeight,
            animal->z + random->nextFloat() * animal->bbWidth * 2 -
                animal->bbWidth,
            xa, ya, za);
    }
    // 4J-PB - Fix for 106869- Customer Encountered: TU12: Content: Gameplay:
    // Breeding animals does not give any Experience Orbs.
    level->addEntity(std::make_shared<ExperienceOrb>(
        level, animal->x, animal->y, animal->z, random->nextInt(7) + 1));
}
