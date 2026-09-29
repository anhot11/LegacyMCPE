#include "TakeFlowerGoal.h"

#include <vector>

#include "java/Random.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"
#include "minecraft/world/entity/ai/goal/OfferFlowerGoal.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/animal/VillagerGolem.h"
#include "minecraft/world/entity/npc/Villager.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/AABB.h"

class Entity;

TakeFlowerGoal::TakeFlowerGoal(Villager* villager) {
    takeFlower = false;
    pickupTick = 0;
    golem = std::weak_ptr<VillagerGolem>();

    this->villager = villager;
    setRequiredControlFlags(Control::MoveControlFlag |
                            Control::LookControlFlag);
}

bool TakeFlowerGoal::canUse() {
    if (villager->getAge() >= 0) return false;
    if (!villager->level->isDay()) return false;

    AABB villager_bb = villager->bb.grow(6, 2, 6);
    std::vector<std::shared_ptr<Entity> >* golems =
        villager->level->getEntitiesOfClass(typeid(VillagerGolem),
                                            &villager_bb);
    if (golems->size() == 0) {
        delete golems;
        return false;
    }

    // for (Entity e : golems)
    for (auto it = golems->begin(); it != golems->end(); ++it) {
        std::shared_ptr<VillagerGolem> vg =
            std::dynamic_pointer_cast<VillagerGolem>(*it);
        if (vg->getOfferFlowerTick() > 0) {
            golem = std::weak_ptr<VillagerGolem>(vg);
            break;
        }
    }
    delete golems;
    return golem.lock() != nullptr;
}

bool TakeFlowerGoal::canContinueToUse() {
    return golem.lock() != nullptr && golem.lock()->getOfferFlowerTick() > 0;
}

void TakeFlowerGoal::start() {
    pickupTick = villager->getRandom()->nextInt(
        (int)(OfferFlowerGoal::OFFER_TICKS * 0.8));
    takeFlower = false;
    golem.lock()->getNavigation()->stop();
}

void TakeFlowerGoal::stop() {
    golem = std::weak_ptr<VillagerGolem>();
    villager->getNavigation()->stop();
}

void TakeFlowerGoal::tick() {
    villager->getLookControl()->setLookAt(golem.lock(), 30, 30);
    if (golem.lock()->getOfferFlowerTick() == pickupTick) {
        villager->getNavigation()->moveTo(golem.lock(), 0.5f);
        takeFlower = true;
    }

    if (takeFlower) {
        if (villager->distanceToSqr(golem.lock()) < 2 * 2) {
            golem.lock()->offerFlower(false);
            villager->getNavigation()->stop();
        }
    }
}
