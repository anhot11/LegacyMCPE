#include "ThrownPotion.h"

#include <math.h>

#include <string>
#include <vector>

#include "java/JavaMath.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/world/effect/MobEffect.h"
#include "minecraft/world/effect/MobEffectInstance.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/projectile/Throwable.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/PotionItem.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/LevelEvent.h"
#include "minecraft/world/phys/AABB.h"
#include "minecraft/world/phys/HitResult.h"
#include "nbt/CompoundTag.h"

class Entity;

const double ThrownPotion::SPLASH_RANGE = 4.0;
const double ThrownPotion::SPLASH_RANGE_SQ =
    ThrownPotion::SPLASH_RANGE * ThrownPotion::SPLASH_RANGE;

void ThrownPotion::_init() {
    // 4J Stu - This function call had to be moved here from the Entity ctor to
    // ensure that the derived version of the function is called
    this->defineSynchedData();

    potionItem = nullptr;
}

ThrownPotion::ThrownPotion(Level* level) : Throwable(level) { _init(); }

ThrownPotion::ThrownPotion(Level* level, std::shared_ptr<LivingEntity> mob,
                           int potionValue)
    : Throwable(level, mob) {
    _init();

    potionItem = std::shared_ptr<ItemInstance>(
        new ItemInstance(Item::potion, 1, potionValue));
}

ThrownPotion::ThrownPotion(Level* level, std::shared_ptr<LivingEntity> mob,
                           std::shared_ptr<ItemInstance> potion)
    : Throwable(level, mob) {
    _init();

    potionItem = potion;
}

ThrownPotion::ThrownPotion(Level* level, double x, double y, double z,
                           int potionValue)
    : Throwable(level, x, y, z) {
    _init();

    potionItem = std::shared_ptr<ItemInstance>(
        new ItemInstance(Item::potion, 1, potionValue));
}

ThrownPotion::ThrownPotion(Level* level, double x, double y, double z,
                           std::shared_ptr<ItemInstance> potion)
    : Throwable(level, x, y, z) {
    _init();

    potionItem = potion;
}

float ThrownPotion::getGravity() { return 0.05f; }

float ThrownPotion::getThrowPower() { return 0.5f; }

float ThrownPotion::getThrowUpAngleOffset() { return -20; }

void ThrownPotion::setPotionValue(int potionValue) {
    if (potionItem == nullptr)
        potionItem = std::make_shared<ItemInstance>(Item::potion, 1, 0);
    potionItem->setAuxValue(potionValue);
}

int ThrownPotion::getPotionValue() {
    if (potionItem == nullptr)
        potionItem = std::make_shared<ItemInstance>(Item::potion, 1, 0);
    return potionItem->getAuxValue();
}

void ThrownPotion::onHit(HitResult* res) {
    if (!level->isClientSide) {
        std::vector<MobEffectInstance*>* mobEffects =
            Item::potion->getMobEffects(potionItem);

        if (mobEffects != nullptr && !mobEffects->empty()) {
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

                        // for (MobEffectInstance effect : mobEffects)
                        for (auto itMEI = mobEffects->begin();
                             itMEI != mobEffects->end(); ++itMEI) {
                            MobEffectInstance* effect = *itMEI;
                            int id = effect->getId();
                            if (MobEffect::effects[id]->isInstantenous()) {
                                MobEffect::effects[id]->applyInstantenousEffect(
                                    getOwner(), e, effect->getAmplifier(),
                                    scale);
                            } else {
                                int duration =
                                    (int)(scale *
                                              (double)effect->getDuration() +
                                          .5);
                                if (duration >
                                    SharedConstants::TICKS_PER_SECOND) {
                                    e->addEffect(new MobEffectInstance(
                                        id, duration, effect->getAmplifier()));
                                }
                            }
                        }
                    }
                }
            }
            delete entitiesOfClass;
        }
        level->levelEvent(LevelEvent::PARTICLES_POTION_SPLASH,
                          (int)Math::round(x), (int)Math::round(y),
                          (int)Math::round(z), getPotionValue());

        remove();
    }
}

void ThrownPotion::readAdditionalSaveData(CompoundTag* tag) {
    Throwable::readAdditionalSaveData(tag);

    if (tag->contains("Potion")) {
        potionItem = ItemInstance::fromTag(tag->getCompound("Potion"));
    } else {
        setPotionValue(tag->getInt("potionValue"));
    }

    if (potionItem == nullptr) remove();
}

void ThrownPotion::addAdditonalSaveData(CompoundTag* tag) {
    Throwable::addAdditonalSaveData(tag);

    if (potionItem != nullptr)
        tag->putCompound("Potion", potionItem->save(new CompoundTag()));
}
