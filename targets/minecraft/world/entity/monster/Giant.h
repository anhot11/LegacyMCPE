#pragma once

#include "Monster.h"
#include "java/Class.h"

class Level;
class Entity;

class Giant : public Monster {
public:
    eINSTANCEOF GetType() { return eTYPE_GIANT; }
    static Entity* create(Level* level) { return new Giant(level); }

    Giant(Level* level);

protected:
    virtual void registerAttributes();

public:
    virtual float getWalkTargetValue(int x, int y, int z);
};
