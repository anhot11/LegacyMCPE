#pragma once
#include <memory>

#include "DefaultDispenseItemBehavior.h"

class Projectile;
class Position;
class Level;

class AbstractProjectileDispenseBehavior : public DefaultDispenseItemBehavior {
public:
    virtual std::shared_ptr<ItemInstance> execute(
        BlockSource* source, std::shared_ptr<ItemInstance> dispensed,
        eOUTCOME& outcome);

protected:
    virtual void playSound(BlockSource* source, eOUTCOME outcome);
    virtual float getUncertainty();
    virtual float getPower();
    virtual std::shared_ptr<Projectile> getProjectile(Level* world,
                                                      Position* position) = 0;
};