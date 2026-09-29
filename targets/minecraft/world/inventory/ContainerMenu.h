#pragma once

#include <memory>

#include "AbstractContainerMenu.h"

class Container;

class ContainerMenu : public AbstractContainerMenu {
private:
    std::shared_ptr<Container> container;
    int containerRows;

public:
    ContainerMenu(std::shared_ptr<Container> inventory,
                  std::shared_ptr<Container> container);

    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual std::shared_ptr<ItemInstance> quickMoveStack(
        std::shared_ptr<Player> player, int slotIndex);
    virtual void removed(std::shared_ptr<Player> player);
    virtual std::shared_ptr<Container> getContainer();

    // 4J ADDED,
    virtual std::shared_ptr<ItemInstance> clicked(
        int slotIndex, int buttonNum, int clickType,
        std::shared_ptr<Player> player, bool looped = false);
};
