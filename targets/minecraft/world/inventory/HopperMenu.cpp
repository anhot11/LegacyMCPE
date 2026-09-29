#include "HopperMenu.h"

#include <vector>

#include "minecraft/world/Container.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"
#include "minecraft/world/inventory/Slot.h"
#include "minecraft/world/item/ItemInstance.h"

HopperMenu::HopperMenu(std::shared_ptr<Container> inventory,
                       std::shared_ptr<Container> hopper) {
    this->hopper = hopper;
    hopper->startOpen();
    int yo = 51;

    for (int x = 0; x < hopper->getContainerSize(); x++) {
        addSlot(new Slot(hopper, x, 44 + x * 18, 20));
    }

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 9; x++) {
            addSlot(
                new Slot(inventory, x + y * 9 + 9, 8 + x * 18, y * 18 + yo));
        }
    }
    for (int x = 0; x < 9; x++) {
        addSlot(new Slot(inventory, x, 8 + x * 18, 58 + yo));
    }
}

bool HopperMenu::stillValid(std::shared_ptr<Player> player) {
    return hopper->stillValid(player);
}

std::shared_ptr<ItemInstance> HopperMenu::quickMoveStack(
    std::shared_ptr<Player> player, int slotIndex) {
    std::shared_ptr<ItemInstance> clicked = nullptr;
    Slot* slot = slots.at(slotIndex);
    if (slot != nullptr && slot->hasItem()) {
        std::shared_ptr<ItemInstance> stack = slot->getItem();
        clicked = stack->copy();

        if (slotIndex < hopper->getContainerSize()) {
            if (!moveItemStackTo(stack, hopper->getContainerSize(),
                                 slots.size(), true)) {
                return nullptr;
            }
        } else {
            if (!moveItemStackTo(stack, 0, hopper->getContainerSize(), false)) {
                return nullptr;
            }
        }
        if (stack->count == 0) {
            slot->set(nullptr);
        } else {
            slot->setChanged();
        }
    }
    return clicked;
}

void HopperMenu::removed(std::shared_ptr<Player> player) {
    AbstractContainerMenu::removed(player);
    hopper->stopOpen();
}

std::shared_ptr<Container> HopperMenu::getContainer() { return hopper; }