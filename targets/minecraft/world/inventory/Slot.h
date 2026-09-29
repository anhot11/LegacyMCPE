#pragma once

#include <memory>

#include "minecraft/world/item/ItemInstance.h"

class Container;
class Icon;
class Player;

class Slot {
private:
    int slot;

public:
    std::shared_ptr<Container> container;

public:
    int index;
    int x, y;

    Slot(std::shared_ptr<Container> container, int slot, int x, int y);
    virtual ~Slot() {}

    void onQuickCraft(std::shared_ptr<ItemInstance> picked,
                      std::shared_ptr<ItemInstance> original);

protected:
    virtual void onQuickCraft(std::shared_ptr<ItemInstance> picked, int count);
    virtual void checkTakeAchievements(std::shared_ptr<ItemInstance> picked);

public:
    void swap(Slot* other);
    virtual void onTake(std::shared_ptr<Player> player,
                        std::shared_ptr<ItemInstance> carried);
    virtual bool mayPlace(std::shared_ptr<ItemInstance> item);
    virtual std::shared_ptr<ItemInstance> getItem();
    virtual bool hasItem();
    virtual void set(std::shared_ptr<ItemInstance> item);
    virtual void setChanged();
    virtual int getMaxStackSize();
    virtual Icon* getNoItemIcon();
    virtual std::shared_ptr<ItemInstance> remove(int c);
    virtual bool isAt(std::shared_ptr<Container> c, int s);
    virtual bool mayPickup(std::shared_ptr<Player> player);
    virtual bool isActive();
    virtual bool mayCombine(std::shared_ptr<ItemInstance> item);  // 4J Added
    virtual std::shared_ptr<ItemInstance> combine(
        std::shared_ptr<ItemInstance> item);  // 4J Added
};