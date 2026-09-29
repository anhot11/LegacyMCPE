#pragma once

#include <format>
#include <memory>
#include <string>

#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/world/Container.h"
#include "nbt/ListTag.h"

class Player;
class CompoundTag;
class ItemInstance;

class ChestTileEntity : public TileEntity, public Container {
public:
    eINSTANCEOF GetType() { return eTYPE_CHESTTILEENTITY; }
    static TileEntity* create() { return new ChestTileEntity(); }

    int getContainerType();  // 4J-Added;

    using TileEntity::setChanged;

private:
    void _init(bool isBonusChest);

public:
    ChestTileEntity(bool isBonusChest = false);            // 4J added param
    ChestTileEntity(int type, bool isBonusChest = false);  // 4J added param
    virtual ~ChestTileEntity();

private:
    std::vector<std::shared_ptr<ItemInstance>>* items;

public:
    bool isBonusChest;  // 4J added
    bool hasCheckedNeighbors;
    std::weak_ptr<ChestTileEntity> n;
    std::weak_ptr<ChestTileEntity> e;
    std::weak_ptr<ChestTileEntity> w;
    std::weak_ptr<ChestTileEntity> s;

    float openness, oOpenness;
    int openCount;

private:
    int tickInterval;

    int type;
    std::string name;

public:
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
    virtual void load(CompoundTag* base);
    virtual void save(CompoundTag* base);
    virtual int getMaxStackSize();
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual void setChanged();
    virtual void clearCache();

private:
    virtual void heyImYourNeighbor(std::shared_ptr<ChestTileEntity> neighbor,
                                   int from);

public:
    virtual void checkNeighbors();

private:
    bool isSameChest(int x, int y, int z);

public:
    virtual void tick();
    virtual bool triggerEvent(int b0, int b1);
    virtual void startOpen();
    virtual void stopOpen();
    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
    virtual void setRemoved();
    virtual int getType();

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();
};