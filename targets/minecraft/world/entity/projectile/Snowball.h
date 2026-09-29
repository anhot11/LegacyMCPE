#pragma once

#include <memory>

#include "Throwable.h"
#include "java/Class.h"

class HitResult;
class Entity;
class Level;
class LivingEntity;

class Snowball : public Throwable {
public:
    eINSTANCEOF GetType() { return eTYPE_SNOWBALL; }
    static Entity* create(Level* level) { return new Snowball(level); }

private:
    void _init();

public:
    Snowball(Level* level);
    Snowball(Level* level, std::shared_ptr<LivingEntity> mob);
    Snowball(Level* level, double x, double y, double z);

protected:
    virtual void onHit(HitResult* res);
};