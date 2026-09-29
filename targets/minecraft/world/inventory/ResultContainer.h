#pragma once

#include <memory>
#include <string>

#include "minecraft/world/Container.h"

class ItemInstance;

class ResultContainer : public Container {
private:
    std::shared_ptr<ItemInstance> items[1];

public:
    // 4J Stu Added a ctor to init items
    ResultContainer();

    virtual unsigned int getContainerSize() override;
    virtual std::shared_ptr<ItemInstance> getItem(unsigned int slot) override;
    virtual std::string getName() override;
    virtual std::string getCustomName() override;
    virtual bool hasCustomName() override;
    virtual std::shared_ptr<ItemInstance> removeItem(unsigned int slot,
                                                     int count) override;
    virtual std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot) override;
    virtual void setItem(unsigned int slot,
                         std::shared_ptr<ItemInstance> item) override;
    virtual int getMaxStackSize() override;
    virtual void setChanged() override;
    virtual bool stillValid(std::shared_ptr<Player> player) override;
    virtual void startOpen() override {}  // TODO Auto-generated method stub
    virtual void stopOpen() override {}   // TODO Auto-generated method stub
    virtual bool canPlaceItem(int slot,
                              std::shared_ptr<ItemInstance> item) override;
};