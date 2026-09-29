#pragma once

#include <memory>

#include "Entity.h"
#include "HangingEntity.h"
#include "java/Class.h"
#include "minecraft/world/item/ItemInstance.h"

class Level;
class Entity;

class ItemFrame : public HangingEntity {
public:
    eINSTANCEOF GetType() { return eTYPE_ITEM_FRAME; };
    static Entity* create(Level* level) { return new ItemFrame(level); }

private:
    static const int DATA_ITEM = 2;
    static const int DATA_ROTATION = 3;

    float dropChance;

private:
    void _init();

public:
    ItemFrame(Level* level);
    ItemFrame(Level* level, int xTile, int yTile, int zTile, int dir);

protected:
    virtual void defineSynchedData();

public:
    virtual int getWidth() { return 9; }
    virtual int getHeight() { return 9; }
    virtual bool shouldRenderAtSqrDistance(double distance);
    virtual void dropItem(std::shared_ptr<Entity> causedBy);

private:
    void removeFramedMap(std::shared_ptr<ItemInstance> item);

public:
    std::shared_ptr<ItemInstance> getItem();
    void setItem(std::shared_ptr<ItemInstance> item);
    int getRotation();
    void setRotation(int rotation);

    virtual void addAdditonalSaveData(CompoundTag* tag);
    virtual void readAdditionalSaveData(CompoundTag* tag);
    virtual bool interact(std::shared_ptr<Player> player);
};