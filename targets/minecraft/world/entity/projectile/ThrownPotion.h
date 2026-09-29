#pragma once

#include <memory>

#include "Throwable.h"
#include "java/Class.h"

class HitResult;
class Entity;
class ItemInstance;
class Level;
class LivingEntity;

class ThrownPotion : public Throwable {
public:
    eINSTANCEOF GetType() { return eTYPE_THROWNPOTION; }
    static Entity* create(Level* level) { return new ThrownPotion(level); }

public:
    static const double SPLASH_RANGE;

private:
    static const double SPLASH_RANGE_SQ;

    std::shared_ptr<ItemInstance> potionItem;

    void _init();

public:
    ThrownPotion(Level* level);
    ThrownPotion(Level* level, std::shared_ptr<LivingEntity> mob,
                 int potionValue);
    ThrownPotion(Level* level, std::shared_ptr<LivingEntity> mob,
                 std::shared_ptr<ItemInstance> potion);
    ThrownPotion(Level* level, double x, double y, double z, int potionValue);
    ThrownPotion(Level* level, double x, double y, double z,
                 std::shared_ptr<ItemInstance> potion);

protected:
    virtual float getGravity();
    virtual float getThrowPower();
    virtual float getThrowUpAngleOffset();

public:
    void setPotionValue(int potionValue);
    int getPotionValue();

protected:
    virtual void onHit(HitResult* res);

public:
    void readAdditionalSaveData(CompoundTag* tag);
    void addAdditonalSaveData(CompoundTag* tag);
};