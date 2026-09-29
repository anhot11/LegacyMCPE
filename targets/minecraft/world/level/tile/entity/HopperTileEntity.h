#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Hopper.h"
#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/item/ItemInstance.h"

class ItemEntity;
class Level;

class HopperTileEntity : public TileEntity, public Hopper {
public:
    eINSTANCEOF GetType() { return eTYPE_HOPPERTILEENTITY; }
    static TileEntity* create() { return new HopperTileEntity(); }
    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();

public:
    static const int MOVE_ITEM_SPEED = 8;

private:
    std::vector<std::shared_ptr<ItemInstance>> items;
    std::string name;
    int cooldownTime;

public:
    HopperTileEntity();
    ~HopperTileEntity();

    virtual void load(CompoundTag* base);
    virtual void save(CompoundTag* base);
    virtual void setChanged();
    virtual unsigned int getContainerSize();
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot,
                                                     int count);
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    virtual void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    virtual std::string getName();
    virtual std::string getCustomName();
    virtual bool hasCustomName();
    virtual void setCustomName(const std::string& name);
    virtual int getMaxStackSize();
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual void startOpen();
    virtual void stopOpen();
    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
    virtual void tick();
    virtual bool tryMoveItems();

private:
    virtual bool ejectItems();

public:
    static bool suckInItems(Hopper* hopper);

private:
    static bool tryTakeInItemFromSlot(Hopper* hopper, Container* container,
                                      int slot, int face);

public:
    static bool addItem(Container* container, std::shared_ptr<ItemEntity> item);
    static std::shared_ptr<ItemInstance> addItem(
        Container* container, std::shared_ptr<ItemInstance> item, int face);

private:
    static bool canPlaceItemInContainer(Container* container,
                                        std::shared_ptr<ItemInstance> item,
                                        int slot, int face);
    static bool canTakeItemFromContainer(Container* container,
                                         std::shared_ptr<ItemInstance> item,
                                         int slot, int face);
    static std::shared_ptr<ItemInstance> tryMoveInItem(
        Container* container, std::shared_ptr<ItemInstance> item, int slot,
        int face);
    virtual std::shared_ptr<Container> getAttachedContainer();

public:
    static std::shared_ptr<Container> getSourceContainer(Hopper* hopper);
    static std::shared_ptr<ItemEntity> getItemAt(Level* level, double xt,
                                                 double yt, double zt);
    static std::shared_ptr<Container> getContainerAt(Level* level, double x,
                                                     double y, double z);

private:
    static bool canMergeItems(std::shared_ptr<ItemInstance> a,
                              std::shared_ptr<ItemInstance> b);

public:
    virtual Level* getLevel();
    virtual double getLevelX();
    virtual double getLevelY();
    virtual double getLevelZ();
    virtual void setCooldown(int time);
    virtual bool isOnCooldown();
};