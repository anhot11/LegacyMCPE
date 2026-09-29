#pragma once

#include <memory>
#include <string>

#include "MinecartContainer.h"
#include "java/Class.h"
#include "minecraft/world/level/tile/entity/Hopper.h"

class Entity;
class Level;

class MinecartHopper : public MinecartContainer, public Hopper {
public:
    eINSTANCEOF GetType() { return eTYPE_MINECART_HOPPER; };
    static Entity* create(Level* level) { return new MinecartHopper(level); }

public:
    static const int MOVE_ITEM_SPEED;

private:
    bool enabled;
    int cooldownTime;

    void _init();

public:
    MinecartHopper(Level* level);
    MinecartHopper(Level* level, double x, double y, double z);

    virtual int getType();
    virtual Tile* getDefaultDisplayTile();
    virtual int getDefaultDisplayOffset();
    virtual unsigned int getContainerSize();
    virtual bool interact(std::shared_ptr<Player> player);
    virtual void activateMinecart(int xt, int yt, int zt, bool state);
    virtual bool isEnabled();
    virtual void setEnabled(bool enabled);
    virtual Level* getLevel();
    virtual double getLevelX();
    virtual double getLevelY();
    virtual double getLevelZ();
    virtual void tick();
    virtual bool suckInItems();
    virtual void destroy(DamageSource* source);

protected:
    virtual void addAdditonalSaveData(CompoundTag* base);
    virtual void readAdditionalSaveData(CompoundTag* base);

public:
    void setCooldown(int time);
    bool isOnCooldown();

    // 4J For Hopper
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot) {
        return MinecartContainer::getItem(slot);
    }
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot,
                                                     int count) {
        return MinecartContainer::removeItem(slot, count);
    }
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot) {
        return MinecartContainer::removeItemNoUpdate(slot);
    }
    virtual void setItem(unsigned int slot,
                         std::shared_ptr<ItemInstance> item) {
        MinecartContainer::setItem(slot, item);
    }
    virtual std::string getName() { return MinecartContainer::getName(); }
    virtual std::string getCustomName() {
        return MinecartContainer::getCustomName();
    }
    virtual bool hasCustomName() { return MinecartContainer::hasCustomName(); }
    virtual int getMaxStackSize() {
        return MinecartContainer::getMaxStackSize();
    }

    virtual void setChanged() { MinecartContainer::setChanged(); }
    virtual bool stillValid(std::shared_ptr<Player> player) {
        return MinecartContainer::stillValid(player);
    }
    virtual void startOpen() { MinecartContainer::startOpen(); }
    virtual void stopOpen() { MinecartContainer::stopOpen(); }
    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item) {
        return MinecartContainer::canPlaceItem(slot, item);
    }
};