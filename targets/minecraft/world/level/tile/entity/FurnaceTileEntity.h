#pragma once

#include <memory>
#include <string>
#include <vector>

#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/world/WorldlyContainer.h"
#include "minecraft/world/level/tile/FurnaceTile.h"

class Player;
class Level;
class ItemInstance;

class FurnaceTileEntity : public TileEntity, public WorldlyContainer {
public:
    eINSTANCEOF GetType() { return eTYPE_FURNACETILEENTITY; }
    static TileEntity* create() { return new FurnaceTileEntity(); }

    using TileEntity::setChanged;

    static const int SLOT_INPUT = 0;
    static const int SLOT_FUEL = 1;
    static const int SLOT_RESULT = 2;

private:
    static const std::vector<int> SLOTS_FOR_UP;
    static const std::vector<int> SLOTS_FOR_DOWN;
    static const std::vector<int> SLOTS_FOR_SIDES;

    static const int BURN_INTERVAL;
    std::vector<std::shared_ptr<ItemInstance>> items;

    // 4J-JEV: Added for 'Renewable Energy' achievement.
    // Should be true iff characoal was consumed whilst cooking the current
    // stack.
    bool m_charcoalUsed;

public:
    int litTime;
    int litDuration;
    int tickCount;

private:
    std::string name;

public:
    // 4J Stu - Need a ctor to initialise member variables
    FurnaceTileEntity();
    virtual ~FurnaceTileEntity();

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
    int getBurnProgress(int max);
    int getLitProgress(int max);
    bool isLit();
    virtual void tick();

private:
    bool canBurn();

public:
    void burn();

    static int getBurnDuration(std::shared_ptr<ItemInstance> itemInstance);
    static bool isFuel(std::shared_ptr<ItemInstance> item);

public:
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual void setChanged();

    void startOpen();
    void stopOpen();

    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
    virtual std::vector<int> getSlotsForFace(int face);
    virtual bool canPlaceItemThroughFace(int slot,
                                         std::shared_ptr<ItemInstance> item,
                                         int face);
    virtual bool canTakeItemThroughFace(int slot,
                                        std::shared_ptr<ItemInstance> item,
                                        int face);

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();

    // 4J-JEV: Added for 'Renewable Energy' achievement.
    bool wasCharcoalUsed() { return m_charcoalUsed; }
};