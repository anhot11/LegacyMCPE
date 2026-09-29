#pragma once

#include "minecraft/world/entity/Creature.h"
#include "minecraft/world/entity/Mob.h"

class Level;

class AmbientCreature : public Mob, public Creature {
public:
    AmbientCreature(Level* level);

    virtual bool canBeLeashed();

protected:
    virtual bool mobInteract(std::shared_ptr<Player> player);
};