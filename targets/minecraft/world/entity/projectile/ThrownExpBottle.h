#pragma once

#include <memory>

#include "Throwable.h"
#include "java/Class.h"

class HitResult;
class Entity;
class Level;
class LivingEntity;

class ThrownExpBottle : public Throwable {
public:
    eINSTANCEOF GetType() { return eTYPE_THROWNEXPBOTTLE; }
    static Entity* create(Level* level) { return new ThrownExpBottle(level); }

public:
    ThrownExpBottle(Level* level);
    ThrownExpBottle(Level* level, std::shared_ptr<LivingEntity> mob);
    ThrownExpBottle(Level* level, double x, double y, double z);

protected:
    virtual float getGravity();
    virtual float getThrowPower();
    virtual float getThrowUpAngleOffset();
    virtual void onHit(HitResult* res);
};