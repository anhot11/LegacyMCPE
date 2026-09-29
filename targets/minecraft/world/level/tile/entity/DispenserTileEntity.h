#pragma once

#include <memory>
#include <string>
#include <vector>

#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/world/Container.h"
#include "nbt/NbtIo.h"

class Player;
class Random;
class Level;
class CompoundTag;
class ItemInstance;

class DispenserTileEntity : public TileEntity, public Container {
public:
    eINSTANCEOF GetType() { return eTYPE_DISPENSERTILEENTITY; }
    static TileEntity* create() { return new DispenserTileEntity(); }

    using TileEntity::setChanged;

private:
    std::vector<std::shared_ptr<ItemInstance>> items;
    Random* random;

protected:
    std::string name;

public:
    DispenserTileEntity();
    virtual ~DispenserTileEntity();

    virtual unsigned int getContainerSize();
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot,
                                                     int count);
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    virtual bool removeProjectile(int itemId);
    virtual int getRandomSlot();
    virtual void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    virtual int addItem(std::shared_ptr<ItemInstance> item);
    virtual std::string getName();
    virtual std::string getCustomName();
    virtual void setCustomName(const std::string& name);
    virtual bool hasCustomName();
    virtual void load(CompoundTag* base);
    virtual void save(CompoundTag* base);
    virtual int getMaxStackSize();
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual void setChanged();

    virtual void startOpen();
    virtual void stopOpen();
    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();
    void AddItemBack(std::shared_ptr<ItemInstance> item, unsigned int slot);
};