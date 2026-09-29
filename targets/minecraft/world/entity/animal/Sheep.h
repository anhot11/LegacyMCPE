#pragma once

#include <stdint.h>

#include <memory>

#include "Animal.h"
#include "java/Class.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"

class Random;
class EatTileGoal;
class CraftingContainer;
class Entity;
class Level;

class Sheep : public Animal {
private:
    class SheepContainer : public AbstractContainerMenu {
        bool stillValid(std::shared_ptr<Player> player) { return false; }
    };

    std::shared_ptr<CraftingContainer> container;

public:
    eINSTANCEOF GetType() { return eTYPE_SHEEP; }
    static Entity* create(Level* level) { return new Sheep(level); }

private:
    static const int EAT_ANIMATION_TICKS =
        SharedConstants::TICKS_PER_SECOND * 2;
    static const int DATA_WOOL_ID = 16;

    int eatAnimationTick;
    EatTileGoal* eatTileGoal;

public:
    static const int COLOR_LENGTH = 16;
    static const float COLOR[COLOR_LENGTH][3];

public:
    Sheep(Level* level);

protected:
    virtual bool useNewAi();
    virtual void newServerAiStep();

public:
    void aiStep();

protected:
    virtual void registerAttributes();
    virtual void defineSynchedData();

public:
    virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
    virtual int getDeathLoot();

public:
    virtual void handleEntityEvent(uint8_t id);

public:
    float getHeadEatPositionScale(float a);
    float getHeadEatAngleScale(float a);

    virtual bool mobInteract(std::shared_ptr<Player> player);
    virtual void addAdditonalSaveData(CompoundTag* tag);
    virtual void readAdditionalSaveData(CompoundTag* tag);

protected:
    virtual int getAmbientSound();
    virtual int getHurtSound();
    virtual int getDeathSound();
    virtual void playStepSound(int xt, int yt, int zt, int t);

public:
    int getColor();
    void setColor(int color);
    bool isSheared();
    void setSheared(bool value);

    static int getSheepColor(Random* random);
    virtual std::shared_ptr<AgableMob> getBreedOffspring(
        std::shared_ptr<AgableMob> target);

    virtual void ate();

    MobGroupData* finalizeMobSpawn(
        MobGroupData* groupData,
        int extraData = 0);  // 4J Added extraData param

private:
    int getOffspringColor(std::shared_ptr<Animal> animal,
                          std::shared_ptr<Animal> partner);
    int getDyeColor(std::shared_ptr<Animal> animal);
};
