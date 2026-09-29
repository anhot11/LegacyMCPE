#pragma once
#include <memory>
#include <string>
#include <vector>

#include "minecraft/world/Container.h"
#include "minecraft/world/item/ItemInstance.h"
#include "nbt/ListTag.h"

class Player;
class CompoundTag;
class Item;
class Tile;
template <class T>
class ListTag;

class Inventory : public Container {
public:
    static const int POP_TIME_DURATION;
    static const int MAX_INVENTORY_STACK_SIZE;

private:
    static const int INVENTORY_SIZE;
    static const int SELECTION_SIZE;

public:
    std::vector<std::shared_ptr<ItemInstance>> items;
    std::vector<std::shared_ptr<ItemInstance>> armor;

    int selected;
    Player* player;  // This is owned by shared_ptrs, but we are owned by it

private:
    std::shared_ptr<ItemInstance> heldItem;
    std::shared_ptr<ItemInstance> carried;

public:
    bool changed;

    Inventory(Player* player);
    ~Inventory();

    std::shared_ptr<ItemInstance> getSelected();
    // 4J-PB - Added for the in-game tooltips
    bool IsHeldItem();
    static int getSelectionSize();

private:
    int getSlot(int tileId);
    int getSlot(int tileId, int data);

    int getSlotWithRemainingSpace(std::shared_ptr<ItemInstance> item);

public:
    int getFreeSlot();
    void grabTexture(int id, int data, bool checkData, bool mayReplace);
    void swapPaint(int wheel);
    int clearInventory(int id, int data);
    void replaceSlot(Item* item, int data);

private:
    int addResource(std::shared_ptr<ItemInstance> itemInstance);

public:
    void tick();
    bool removeResource(int type);

    // 4J-PB added to get the right resource from the inventory for removal
    bool removeResource(int type, int iAuxVal);
    void removeResources(
        std::shared_ptr<ItemInstance> item);  // 4J Added for trading

    // 4J-Stu added to the get the item that would be affected by the
    // removeResource functions
    std::shared_ptr<ItemInstance> getResourceItem(int type);
    std::shared_ptr<ItemInstance> getResourceItem(int type, int iAuxVal);

    bool hasResource(int type);
    void swapSlots(int from, int to);
    bool add(std::shared_ptr<ItemInstance> item);
    std::shared_ptr<ItemInstance> removeItem(unsigned int slot, int count);
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    float getDestroySpeed(Tile* tile);
    ListTag<CompoundTag>* save(ListTag<CompoundTag>* listTag);
    void load(ListTag<CompoundTag>* inventoryList);
    unsigned int getContainerSize();
    std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    std::string getName();
    std::string getCustomName();
    bool hasCustomName();
    int getMaxStackSize();
    bool canDestroy(Tile* tile);
    std::shared_ptr<ItemInstance> getArmor(int layer);
    int getArmorValue();
    void hurtArmor(float dmg);
    void dropAll();
    void setChanged();
    bool isSame(std::shared_ptr<Inventory> copy);

private:
    bool isSame(std::shared_ptr<ItemInstance> a,
                std::shared_ptr<ItemInstance> b);

public:
    std::shared_ptr<Inventory> copy();
    void setCarried(std::shared_ptr<ItemInstance> carried);
    std::shared_ptr<ItemInstance> getCarried();
    bool stillValid(std::shared_ptr<Player> player);
    bool contains(std::shared_ptr<ItemInstance> itemInstance);
    virtual void startOpen();
    virtual void stopOpen();
    bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
    void replaceWith(std::shared_ptr<Inventory> other);

    int countMatches(std::shared_ptr<ItemInstance> itemInstance);  // 4J Added
};