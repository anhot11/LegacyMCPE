#pragma once

#include <memory>
#include <string>

#include "Projectile.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/LivingEntity.h"

class Mob;
class HitResult;
class Level;

class Throwable : public Entity, public Projectile {
private:
    int xTile;
    int yTile;
    int zTile;
    int lastTile;

protected:
    bool inGround;

public:
    int shakeTime;

    std::shared_ptr<LivingEntity> owner;

private:
    std::string ownerName;
    int life;
    int flightTime;

    void _throwableInit();

public:
    Throwable(Level* level);

protected:
    virtual void defineSynchedData();

public:
    virtual bool shouldRenderAtSqrDistance(double distance);

    Throwable(Level* level, std::shared_ptr<LivingEntity> mob);
    Throwable(Level* level, double x, double y, double z);

protected:
    virtual float getThrowPower();
    virtual float getThrowUpAngleOffset();

public:
    virtual void shoot(double xd, double yd, double zd, float pow,
                       float uncertainty);
    virtual void lerpMotion(double xd, double yd, double zd);
    virtual void tick();

protected:
    virtual float getGravity();
    virtual void onHit(HitResult* res) = 0;

public:
    virtual void addAdditonalSaveData(CompoundTag* tag);
    virtual void readAdditionalSaveData(CompoundTag* tag);
    virtual float getShadowHeightOffs();
    virtual std::shared_ptr<LivingEntity> getOwner();
};