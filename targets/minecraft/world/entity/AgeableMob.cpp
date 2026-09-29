#include "AgeableMob.h"

#include <string>

#include "SyncedEntityData.h"
#include "java/Class.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/world/entity/EntityIO.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/player/Abilities.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/SpawnEggItem.h"
#include "minecraft/world/level/Level.h"
#include "nbt/CompoundTag.h"

class Entity;

AgableMob::AgableMob(Level* level) : PathfinderMob(level) {
    registeredBBWidth = -1;
    registeredBBHeight = 0;
}

bool AgableMob::mobInteract(std::shared_ptr<Player> player) {
    std::shared_ptr<ItemInstance> item = player->inventory->getSelected();

    if (item != nullptr && item->id == Item::spawnEgg_Id) {
        if (!level->isClientSide) {
            eINSTANCEOF classToSpawn = EntityIO::getClass(item->getAuxValue());
            if (classToSpawn != eTYPE_NOTSET &&
                (classToSpawn & eTYPE_AGABLE_MOB) == eTYPE_AGABLE_MOB &&
                classToSpawn == GetType())  // 4J Added GetType() check to only
                                            // spawn same type
            {
                int error;
                std::shared_ptr<Entity> result =
                    SpawnEggItem::canSpawn(item->getAuxValue(), level, &error);

                if (result != nullptr) {
                    std::shared_ptr<AgableMob> offspring =
                        getBreedOffspring(std::dynamic_pointer_cast<AgableMob>(
                            shared_from_this()));
                    if (offspring != nullptr) {
                        offspring->setAge(BABY_START_AGE);
                        offspring->moveTo(x, y, z, 0, 0);

                        level->addEntity(offspring);

                        if (!player->abilities.instabuild) {
                            item->count--;

                            if (item->count <= 0) {
                                player->inventory->setItem(
                                    player->inventory->selected, nullptr);
                            }
                        }
                    }
                } else {
                    SpawnEggItem::DisplaySpawnError(player, error);
                }
            }
        }
        return true;
    }

    return false;
}

void AgableMob::defineSynchedData() {
    PathfinderMob::defineSynchedData();
    entityData->define(DATA_AGE_ID, 0);
}

int AgableMob::getAge() { return entityData->getInteger(DATA_AGE_ID); }

void AgableMob::ageUp(int seconds) {
    int age = getAge();
    age += seconds * SharedConstants::TICKS_PER_SECOND;
    if (age > 0) {
        age = 0;
    }
    setAge(age);
}

void AgableMob::setAge(int age) {
    entityData->set(DATA_AGE_ID, age);
    updateSize(isBaby());
}

void AgableMob::addAdditonalSaveData(CompoundTag* tag) {
    PathfinderMob::addAdditonalSaveData(tag);
    tag->putInt("Age", getAge());
}

void AgableMob::readAdditionalSaveData(CompoundTag* tag) {
    PathfinderMob::readAdditionalSaveData(tag);
    setAge(tag->getInt("Age"));
}

void AgableMob::aiStep() {
    PathfinderMob::aiStep();

    if (level->isClientSide) {
        updateSize(isBaby());
    } else {
        int age = getAge();
        if (age < 0) {
            age++;
            setAge(age);
        } else if (age > 0) {
            age--;
            setAge(age);
        }
    }
}

bool AgableMob::isBaby() { return getAge() < 0; }

void AgableMob::updateSize(bool isBaby) {
    internalSetSize(isBaby ? .5f : 1.0f);
}

void AgableMob::setSize(float w, float h) {
    bool inited = registeredBBWidth > 0;

    registeredBBWidth = w;
    registeredBBHeight = h;

    if (!inited) {
        internalSetSize(1.0f);
    }
}

void AgableMob::internalSetSize(float scale) {
    PathfinderMob::setSize(registeredBBWidth * scale,
                           registeredBBHeight * scale);
}
