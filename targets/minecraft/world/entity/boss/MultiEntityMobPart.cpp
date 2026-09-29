#include "MultiEntityMobPart.h"

#include "MultiEntityMob.h"
#include "minecraft/world/entity/Entity.h"

MultiEntityMobPart::MultiEntityMobPart(
    std::shared_ptr<MultiEntityMob> parentMob, const std::string& id, float w,
    float h)
    : Entity(parentMob->getLevel()), parentMob(parentMob), id(id) {
    // 4J Stu - This function call had to be moved here from the Entity ctor to
    // ensure that the derived version of the function is called
    this->defineSynchedData();

    setSize(w, h);
}

void MultiEntityMobPart::defineSynchedData() {}

void MultiEntityMobPart::readAdditionalSaveData(CompoundTag* tag) {}

void MultiEntityMobPart::addAdditonalSaveData(CompoundTag* tag) {}

bool MultiEntityMobPart::isPickable() { return true; }

bool MultiEntityMobPart::hurt(DamageSource* source, float damage) {
    return parentMob.lock()->hurt(
        std::dynamic_pointer_cast<MultiEntityMobPart>(shared_from_this()),
        source, damage);
}

bool MultiEntityMobPart::is(std::shared_ptr<Entity> other) {
    return shared_from_this() == other ||
           parentMob.lock() == std::dynamic_pointer_cast<MultiEntityMob>(other);
}