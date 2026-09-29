#pragma once

#include <memory>

#include "AbstractContainerMenu.h"
#include "minecraft/world/Container.h"

class HopperMenu : public AbstractContainerMenu {
private:
    std::shared_ptr<Container> hopper;

public:
    static const int CONTENTS_SLOT_START = 0;
    static const int INV_SLOT_START = CONTENTS_SLOT_START + 5;
    static const int INV_SLOT_END = INV_SLOT_START + 9 * 3;
    static const int USE_ROW_SLOT_START = INV_SLOT_END;
    static const int USE_ROW_SLOT_END = USE_ROW_SLOT_START + 9;

public:
    HopperMenu(std::shared_ptr<Container> inventory,
               std::shared_ptr<Container> hopper);

    bool stillValid(std::shared_ptr<Player> player);
    std::shared_ptr<ItemInstance> quickMoveStack(std::shared_ptr<Player> player,
                                                 int slotIndex);
    void removed(std::shared_ptr<Player> player);
    std::shared_ptr<Container> getContainer();
};