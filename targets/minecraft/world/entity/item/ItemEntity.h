#pragma once
#include <memory>
#include <string>

#include "java/Class.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/item/ItemInstance.h"

class Player;
class Level;

class ItemEntity : public Entity {
public:
    eINSTANCEOF GetType() { return eTYPE_ITEMENTITY; }
    static Entity* create(Level* level) { return new ItemEntity(level); }

private:
    static const int DATA_ITEM = 10;

    static const int LIFETIME =
        5 * 60 * SharedConstants::TICKS_PER_SECOND;  // Five miniutes.

    std::string thrower;

    // 4J Added
    void _init();
    void _init(Level* level, double x, double y, double z);

public:
    int age;
    int throwTime;

private:
    int health;

public:
    float bobOffs;

    ItemEntity(Level* level, double x, double y, double z);
    ItemEntity(Level* level, double x, double y, double z,
               std::shared_ptr<ItemInstance> item);

protected:
    virtual bool makeStepSound();

public:
    ItemEntity(Level* level);

protected:
    virtual void defineSynchedData();

public:
    virtual void tick();

private:
    void mergeWithNeighbours();

public:
    bool merge(std::shared_ptr<ItemEntity> target);
    void setShortLifeTime();
    virtual bool updateInWaterState();

protected:
    virtual void burn(int dmg);

public:
    virtual bool hurt(DamageSource* source, float damage);
    virtual void addAdditonalSaveData(CompoundTag* entityTag);
    virtual void readAdditionalSaveData(CompoundTag* tag);
    virtual void playerTouch(std::shared_ptr<Player> player);

    virtual std::string getAName();
    virtual void changeDimension(int i);
    std::shared_ptr<ItemInstance> getItem();
    void setItem(std::shared_ptr<ItemInstance> item);
    virtual bool isAttackable();

    void setThrower(const std::string& thrower);
    std::string getThrower();
};
