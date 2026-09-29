#include "MerchantMenu.h"

#include <vector>

#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"
#include "minecraft/world/inventory/MerchantContainer.h"
#include "minecraft/world/inventory/MerchantResultSlot.h"
#include "minecraft/world/inventory/Slot.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/trading/Merchant.h"
#include "minecraft/world/level/Level.h"

MerchantMenu::MerchantMenu(std::shared_ptr<Inventory> inventory,
                           std::shared_ptr<Merchant> merchant, Level* level) {
    trader = merchant;
    this->level = level;

    tradeContainer = std::shared_ptr<MerchantContainer>(
        new MerchantContainer(std::dynamic_pointer_cast<Player>(
                                  inventory->player->shared_from_this()),
                              merchant));
    addSlot(new Slot(tradeContainer, PAYMENT1_SLOT, SELLSLOT1_X, ROW2_Y));
    addSlot(new Slot(tradeContainer, PAYMENT2_SLOT, SELLSLOT2_X, ROW2_Y));
    addSlot(new MerchantResultSlot(inventory->player, merchant, tradeContainer,
                                   RESULT_SLOT, BUYSLOT_X, ROW2_Y));

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 9; x++) {
            addSlot(
                new Slot(inventory, x + y * 9 + 9, 8 + x * 18, 84 + y * 18));
        }
    }
    for (int x = 0; x < 9; x++) {
        addSlot(new Slot(inventory, x, 8 + x * 18, 142));
    }
}

std::shared_ptr<MerchantContainer> MerchantMenu::getTradeContainer() {
    return tradeContainer;
}

void MerchantMenu::addSlotListener(ContainerListener* listener) {
    AbstractContainerMenu::addSlotListener(listener);
}

void MerchantMenu::broadcastChanges() {
    AbstractContainerMenu::broadcastChanges();
}

// 4J used to take a shared_ptr<Container> but wasn't using it, so removed to
// simplify things
void MerchantMenu::slotsChanged() {
    tradeContainer->updateSellItem();
    AbstractContainerMenu::slotsChanged();
}

void MerchantMenu::setSelectionHint(int hint) {
    tradeContainer->setSelectionHint(hint);
}

void MerchantMenu::setData(int id, int value) {}

bool MerchantMenu::stillValid(std::shared_ptr<Player> player) {
    return trader->getTradingPlayer() == player;
}

std::shared_ptr<ItemInstance> MerchantMenu::quickMoveStack(
    std::shared_ptr<Player> player, int slotIndex) {
    std::shared_ptr<ItemInstance> clicked = nullptr;
    Slot* slot = nullptr;

    if (slotIndex < slots.size()) slot = slots.at(slotIndex);
    if (slot != nullptr && slot->hasItem()) {
        std::shared_ptr<ItemInstance> stack = slot->getItem();
        clicked = stack->copy();

        if (slotIndex == RESULT_SLOT) {
            if (!moveItemStackTo(stack, INV_SLOT_START, USE_ROW_SLOT_END,
                                 true)) {
                return nullptr;
            }
            slot->onQuickCraft(stack, clicked);
        } else if (slotIndex == PAYMENT1_SLOT || slotIndex == PAYMENT2_SLOT) {
            if (!moveItemStackTo(stack, INV_SLOT_START, USE_ROW_SLOT_END,
                                 false)) {
                return nullptr;
            }
        } else if (slotIndex >= INV_SLOT_START && slotIndex < INV_SLOT_END) {
            if (!moveItemStackTo(stack, USE_ROW_SLOT_START, USE_ROW_SLOT_END,
                                 false)) {
                return nullptr;
            }
        } else if (slotIndex >= USE_ROW_SLOT_START &&
                   slotIndex < USE_ROW_SLOT_END) {
            if (!moveItemStackTo(stack, INV_SLOT_START, INV_SLOT_END, false)) {
                return nullptr;
            }
        }
        if (stack->count == 0) {
            slot->set(nullptr);
        } else {
            slot->setChanged();
        }
        if (stack->count == clicked->count) {
            return nullptr;
        } else {
            slot->onTake(player, stack);
        }
    }
    return clicked;
}

void MerchantMenu::removed(std::shared_ptr<Player> player) {
    AbstractContainerMenu::removed(player);
    trader->setTradingPlayer(nullptr);

    AbstractContainerMenu::removed(player);
    if (level->isClientSide) return;

    std::shared_ptr<ItemInstance> item =
        tradeContainer->removeItemNoUpdate(PAYMENT1_SLOT);
    if (item) {
        player->drop(item);
    }
    item = tradeContainer->removeItemNoUpdate(PAYMENT2_SLOT);
    if (item != nullptr) {
        player->drop(item);
    }
}

std::shared_ptr<Merchant> MerchantMenu::getMerchant() { return trader; }