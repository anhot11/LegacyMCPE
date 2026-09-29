#pragma once

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "minecraft/world/Container.h"
#include "minecraft/world/item/ItemInstance.h"

class AbstractContainerMenu;

class CraftingContainer : public Container {
private:
    std::vector<std::shared_ptr<ItemInstance>>* items;
    unsigned int width;
    AbstractContainerMenu* menu;

public:
    CraftingContainer(AbstractContainerMenu* menu, unsigned int w,
                      unsigned int h);
    virtual ~CraftingContainer();

    virtual unsigned int getContainerSize();
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    std::shared_ptr<ItemInstance> getItem(unsigned int x, unsigned int y);
    virtual std::string getName();
    virtual std::string getCustomName();
    virtual bool hasCustomName();
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot,
                                                     int count);
    virtual void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    virtual int getMaxStackSize();
    virtual void setChanged();
    bool stillValid(std::shared_ptr<Player> player);

    void startOpen() {}  // TODO Auto-generated method stub
    void stopOpen() {}   // TODO Auto-generated method stub

    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
};