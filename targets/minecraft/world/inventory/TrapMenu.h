#pragma once

#include <memory>

#include "AbstractContainerMenu.h"

class DispenserTileEntity;
class Container;

class TrapMenu : public AbstractContainerMenu {
private:
    static const int INV_SLOT_START = 9;
    static const int INV_SLOT_END = INV_SLOT_START + 9 * 3;
    static const int USE_ROW_SLOT_START = INV_SLOT_END;
    static const int USE_ROW_SLOT_END = USE_ROW_SLOT_START + 9;

private:
    std::shared_ptr<DispenserTileEntity> trap;

public:
    TrapMenu(std::shared_ptr<Container> inventory,
             std::shared_ptr<DispenserTileEntity> trap);

    virtual bool stillValid(std::shared_ptr<Player> player) override;
    virtual std::shared_ptr<ItemInstance> quickMoveStack(
        std::shared_ptr<Player> player, int slotIndex) override;
};