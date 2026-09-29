#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Minecart.h"
#include "minecraft/world/Container.h"

class ItemInstance;
class Level;

class MinecartContainer : public Minecart, public virtual Container {
private:
    std::vector<std::shared_ptr<ItemInstance>> items;
    bool dropEquipment;

    void _init();

public:
    MinecartContainer(Level* level);
    MinecartContainer(Level* level, double x, double y, double z);

    virtual void destroy(DamageSource* source);
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot,
                                                     int count);
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    virtual void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    virtual void setChanged();
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual void startOpen();
    virtual void stopOpen();
    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
    virtual std::string getName();
    virtual int getMaxStackSize();
    virtual void changeDimension(int i);
    virtual void remove();

protected:
    virtual void addAdditonalSaveData(CompoundTag* base);
    virtual void readAdditionalSaveData(CompoundTag* base);

public:
    virtual bool interact(std::shared_ptr<Player> player);

protected:
    virtual void applyNaturalSlowdown();

public:
    // 4J Stu - For container
    virtual bool hasCustomName() { return Minecart::hasCustomName(); }
    virtual std::string getCustomName() { return Minecart::getCustomName(); }
};