#pragma once

#include <memory>

class Level;
class MultiEntityMobPart;
class DamageSource;

class MultiEntityMob {
public:
    virtual Level* getLevel() = 0;
    virtual bool hurt(std::shared_ptr<MultiEntityMobPart> MultiEntityMobPart,
                      DamageSource* source, float damage) = 0;
};