#include "FollowParentGoal.h"

#include <limits>
#include <vector>

#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/animal/Animal.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/AABB.h"

class Entity;

FollowParentGoal::FollowParentGoal(Animal* animal, double speedModifier) {
    timeToRecalcPath = 0;

    this->animal = animal;
    this->speedModifier = speedModifier;
}

bool FollowParentGoal::canUse() {
    if (animal->getAge() >= 0) return false;

    AABB grown_bb = animal->bb.grow(8, 4, 8);
    std::vector<std::shared_ptr<Entity> >* parents =
        animal->level->getEntitiesOfClass(typeid(*animal), &grown_bb);

    std::shared_ptr<Animal> closest = nullptr;
    double closestDistSqr = std::numeric_limits<double>::max();
    for (auto it = parents->begin(); it != parents->end(); ++it) {
        std::shared_ptr<Animal> parent = std::dynamic_pointer_cast<Animal>(*it);
        if (parent->getAge() < 0) continue;
        double distSqr = animal->distanceToSqr(parent);
        if (distSqr > closestDistSqr) continue;
        closestDistSqr = distSqr;
        closest = parent;
    }
    delete parents;

    if (closest == nullptr) return false;
    if (closestDistSqr < 3 * 3) return false;
    parent = std::weak_ptr<Animal>(closest);
    return true;
}

bool FollowParentGoal::canContinueToUse() {
    if (parent.lock() == nullptr || !parent.lock()->isAlive()) return false;
    double distSqr = animal->distanceToSqr(parent.lock());
    if (distSqr < 3 * 3 || distSqr > 16 * 16) return false;
    return true;
}

void FollowParentGoal::start() { timeToRecalcPath = 0; }

void FollowParentGoal::stop() { parent = std::weak_ptr<Animal>(); }

void FollowParentGoal::tick() {
    if (--timeToRecalcPath > 0) return;
    timeToRecalcPath = 10;
    animal->getNavigation()->moveTo(parent.lock(), speedModifier);
}
