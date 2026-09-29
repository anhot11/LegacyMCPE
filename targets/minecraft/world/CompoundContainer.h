#pragma once

#include <memory>
#include <string>

#include "Container.h"
#include "minecraft/world/Container.h"

class Player;

class CompoundContainer : public Container {
private:
    int name;
    std::shared_ptr<Container> c1, c2;

public:
    CompoundContainer(int name, std::shared_ptr<Container> c1,
                      std::shared_ptr<Container> c2);

    virtual int getContainerType();
    virtual unsigned int getContainerSize();
    virtual bool contains(std::shared_ptr<Container> c);
    virtual std::string getName();
    virtual std::string getCustomName();
    virtual bool hasCustomName();
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot, int i);
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    virtual void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    virtual int getMaxStackSize();
    virtual void setChanged();
    virtual bool stillValid(std::shared_ptr<Player> player);

    virtual void startOpen();
    virtual void stopOpen();
    virtual bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
};