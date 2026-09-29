#include "Sensing.h"

#include "minecraft/world/entity/Mob.h"

class Entity;

Sensing::Sensing(Mob* mob) { this->mob = mob; }

void Sensing::tick() {
    seen.clear();
    unseen.clear();
}

bool Sensing::canSee(std::shared_ptr<Entity> target) {
    // if ( find(seen.begin(), seen.end(), target) != seen.end() ) return true;
    // if ( find(unseen.begin(), unseen.end(), target) != unseen.end()) return
    // false;
    for (auto it = seen.begin(); it != seen.end(); ++it) {
        if (target == (*it).lock()) return true;
    }
    for (auto it = unseen.begin(); it != unseen.end(); ++it) {
        if (target == (*it).lock()) return false;
    }

    // util.Timer.push("canSee");
    bool canSee = mob->canSee(target);
    // util.Timer.pop();
    if (canSee)
        seen.push_back(std::weak_ptr<Entity>(target));
    else
        unseen.push_back(std::weak_ptr<Entity>(target));
    return canSee;
}