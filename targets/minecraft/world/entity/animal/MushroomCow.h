#pragma once

#include "Cow.h"
#include "java/Class.h"

class Entity;
class Level;

class MushroomCow : public Cow {
public:
    eINSTANCEOF GetType() { return eTYPE_MUSHROOMCOW; }
    static Entity* create(Level* level) { return new MushroomCow(level); }

public:
    MushroomCow(Level* level);

    virtual bool mobInteract(std::shared_ptr<Player> player);
    virtual bool canSpawn();  // 4J added
    virtual std::shared_ptr<AgableMob> getBreedOffspring(
        std::shared_ptr<AgableMob> target);
};