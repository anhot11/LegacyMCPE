#pragma once
#include <memory>
#include <string>
#include <vector>

#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/world/WorldlyContainer.h"

class ItemInstance;

class BrewingStandTileEntity : public TileEntity, public WorldlyContainer {
public:
    eINSTANCEOF GetType() { return eTYPE_BREWINGSTANDTILEENTITY; }
    static TileEntity* create() { return new BrewingStandTileEntity(); }

    static const int INGREDIENT_SLOT = 3;

private:
    std::vector<std::shared_ptr<ItemInstance>> items;
    static std::vector<int> SLOTS_FOR_UP;
    static std::vector<int> SLOTS_FOR_OTHER_FACES;

    int brewTime;
    int lastPotionCount;
    int ingredientId;
    std::string name;

public:
    BrewingStandTileEntity();
    ~BrewingStandTileEntity();
    virtual std::string getName();
    virtual std::string getCustomName();
    virtual bool hasCustomName();
    virtual void setCustomName(const std::string& name);
    virtual unsigned int getContainerSize();
    virtual void tick();

    int getBrewTime();

private:
    bool isBrewable();
    void doBrew();

    int applyIngredient(int currentBrew,
                        std::shared_ptr<ItemInstance> ingredient);

public:
    virtual void load(CompoundTag* base);
    virtual void save(CompoundTag* base);
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot, int i);
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    virtual void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    virtual int getMaxStackSize();
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual void startOpen();
    virtual void stopOpen();
    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
    virtual void setBrewTime(int value);
    virtual void setChanged() { TileEntity::setChanged(); }  // 4J added
    int getPotionBits();
    virtual std::vector<int> getSlotsForFace(int face);
    virtual bool canPlaceItemThroughFace(int slot,
                                         std::shared_ptr<ItemInstance> item,
                                         int face);
    virtual bool canTakeItemThroughFace(int slot,
                                        std::shared_ptr<ItemInstance> item,
                                        int face);

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();
};