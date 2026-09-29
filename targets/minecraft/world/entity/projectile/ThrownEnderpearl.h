#pragma once

#include <memory>

#include "Throwable.h"
#include "java/Class.h"

class HitResult;
class Entity;
class Level;
class LivingEntity;

class ThrownEnderpearl : public Throwable {
public:
    eINSTANCEOF GetType() { return eTYPE_THROWNENDERPEARL; }
    static Entity* create(Level* level) { return new ThrownEnderpearl(level); }

    ThrownEnderpearl(Level* level);
    ThrownEnderpearl(Level* level, std::shared_ptr<LivingEntity> mob);
    ThrownEnderpearl(Level* level, double x, double y, double z);

protected:
    virtual void onHit(HitResult* res);
};