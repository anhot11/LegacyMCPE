#include "MakeLoveGoal.h"

#include "java/Class.h"
#include "java/Random.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/EntityEvent.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/ai/village/Village.h"
#include "minecraft/world/entity/ai/village/Villages.h"
#include "minecraft/world/entity/npc/Villager.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/AABB.h"

MakeLoveGoal::MakeLoveGoal(Villager* villager) {
    village = std::weak_ptr<Village>();
    partner = std::weak_ptr<Villager>();
    loveMakingTime = 0;

    this->villager = villager;
    level = villager->level;
    setRequiredControlFlags(Control::MoveControlFlag |
                            Control::LookControlFlag);
}

bool MakeLoveGoal::canUse() {
    if (villager->getAge() != 0) return false;
    if (villager->getRandom()->nextInt(500) != 0) return false;

    village = level->villages->getClosestVillage(Mth::floor(villager->x),
                                                 Mth::floor(villager->y),
                                                 Mth::floor(villager->z), 0);
    if (village.lock() == nullptr) return false;
    if (!villageNeedsMoreVillagers()) return false;

    AABB villager_bb = villager->bb.grow(8, 3, 8);
    std::shared_ptr<Entity> mate = level->getClosestEntityOfClass(
        typeid(Villager), &villager_bb, villager->shared_from_this());
    if (mate == nullptr) return false;

    partner =
        std::weak_ptr<Villager>(std::dynamic_pointer_cast<Villager>(mate));
    if (partner.lock()->getAge() != 0) return false;

    return true;
}

void MakeLoveGoal::start() {
    loveMakingTime = 300;
    villager->setInLove(true);
}

void MakeLoveGoal::stop() {
    village = std::weak_ptr<Village>();
    partner = std::weak_ptr<Villager>();
    villager->setInLove(false);
}

bool MakeLoveGoal::canContinueToUse() {
    return partner.lock() != nullptr && loveMakingTime >= 0 &&
           villageNeedsMoreVillagers() && villager->getAge() == 0;
}

void MakeLoveGoal::tick() {
    --loveMakingTime;
    villager->getLookControl()->setLookAt(partner.lock(), 10, 30);

    if (villager->distanceToSqr(partner.lock()) > 1.5 * 1.5) {
        villager->getNavigation()->moveTo(partner.lock(), 0.25f);
    } else {
        if (loveMakingTime == 0 && partner.lock()->isInLove()) breed();
    }

    if (villager->getRandom()->nextInt(35) == 0)
        level->broadcastEntityEvent(villager->shared_from_this(),
                                    EntityEvent::LOVE_HEARTS);
}

bool MakeLoveGoal::villageNeedsMoreVillagers() {
    std::shared_ptr<Village> _village = village.lock();
    if (_village == nullptr) return false;

    if (!_village->isBreedTimerOk()) {
        return false;
    }

    int idealSize = (int)((float)_village->getDoorCount() * 0.35);
    // System.out.println("idealSize: " + idealSize + " pop: " +
    // village.getPopulationSize());
    return _village->getPopulationSize() < idealSize;
}

void MakeLoveGoal::breed() {
    // 4J Stu - This sets a timer that stops these villagers from trying to
    // breed again We should do this even if breeding fails due to vilalger
    // count to stop them continually trying to breed
    partner.lock()->setAge(5 * 60 * 20);
    villager->setAge(5 * 60 * 20);
    // 4J - added limit to number of animals that can be bred
    if (level->canCreateMore(eTYPE_VILLAGER, Level::eSpawnType_Breed)) {
        std::shared_ptr<Villager> child = std::dynamic_pointer_cast<Villager>(
            villager->getBreedOffspring(partner.lock()));
        child->setAge(-20 * 60 * 20);
        child->moveTo(villager->x, villager->y, villager->z, 0, 0);
        level->addEntity(child);
        level->broadcastEntityEvent(child, EntityEvent::LOVE_HEARTS);
    }
}
