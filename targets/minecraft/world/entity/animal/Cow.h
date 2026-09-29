#pragma once

#include "Animal.h"
#include "java/Class.h"

class Player;
class Level;
class Entity;

class Cow : public Animal {
public:
    eINSTANCEOF GetType() { return eTYPE_COW; }
    static Entity* create(Level* level) { return new Cow(level); }

public:
    Cow(Level* level);
    virtual bool useNewAi();

protected:
    virtual void registerAttributes();
    virtual int getAmbientSound();
    virtual int getHurtSound();
    virtual int getDeathSound();
    virtual float getSoundVolume();
    virtual int getDeathLoot();
    virtual void playStepSound(int xt, int yt, int zt, int t);
    virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
    virtual bool mobInteract(std::shared_ptr<Player> player);
    virtual std::shared_ptr<AgableMob> getBreedOffspring(
        std::shared_ptr<AgableMob> target);
};
