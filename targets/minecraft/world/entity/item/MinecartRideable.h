#pragma once

#include "Minecart.h"
#include "java/Class.h"
#include "minecraft/world/entity/item/Minecart.h"

class Entity;
class Level;

class MinecartRideable : public Minecart {
public:
    eINSTANCEOF GetType() { return eTYPE_MINECART_RIDEABLE; };
    static Entity* create(Level* level) { return new MinecartRideable(level); }

public:
    MinecartRideable(Level* level);
    MinecartRideable(Level* level, double x, double y, double z);

    virtual bool interact(std::shared_ptr<Player> player);
    virtual int getType();
};