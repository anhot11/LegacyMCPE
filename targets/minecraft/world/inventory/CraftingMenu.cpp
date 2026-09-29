#include "CraftingMenu.h"

#include <vector>

#include "CraftingContainer.h"
#include "ResultContainer.h"
#include "ResultSlot.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"
#include "minecraft/world/inventory/Slot.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/crafting/Recipes.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"

const int CraftingMenu::RESULT_SLOT = 0;
const int CraftingMenu::CRAFT_SLOT_START = 1;
const int CraftingMenu::CRAFT_SLOT_END = CraftingMenu::CRAFT_SLOT_START + 9;
const int CraftingMenu::INV_SLOT_START = CraftingMenu::CRAFT_SLOT_END;
const int CraftingMenu::INV_SLOT_END = CraftingMenu::INV_SLOT_START + 9 * 3;
const int CraftingMenu::USE_ROW_SLOT_START = CraftingMenu::INV_SLOT_END;
const int CraftingMenu::USE_ROW_SLOT_END = CraftingMenu::USE_ROW_SLOT_START + 9;

CraftingMenu::CraftingMenu(std::shared_ptr<Inventory> inventory, Level* level,
                           int xt, int yt, int zt)
    : AbstractContainerMenu() {
    craftSlots = std::make_shared<CraftingContainer>(this, 3, 3);
    resultSlots = std::make_shared<ResultContainer>();

    this->level = level;
    x = xt;
    y = yt;
    z = zt;
    addSlot(new ResultSlot(inventory->player, craftSlots, resultSlots, 0,
                           120 + 4, 31 + 4));

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            addSlot(new Slot(craftSlots, x + y * 3, 30 + x * 18, 17 + y * 18));
        }
    }

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 9; x++) {
            addSlot(
                new Slot(inventory, x + y * 9 + 9, 8 + x * 18, 84 + y * 18));
        }
    }
    for (int x = 0; x < 9; x++) {
        addSlot(new Slot(inventory, x, 8 + x * 18, 142));
    }

    slotsChanged();  // 4J - removed craftSlots parameter, see comment below
}

void CraftingMenu::slotsChanged()  // 4J used to take a shared_ptr<Container>
                                   // but wasn't using it, so removed to
                                   // simplify things
{
    resultSlots->setItem(0,
                         Recipes::getInstance()->getItemFor(craftSlots, level));
}

void CraftingMenu::removed(std::shared_ptr<Player> player) {
    AbstractContainerMenu::removed(player);
    if (level->isClientSide) return;

    for (int i = 0; i < 9; i++) {
        std::shared_ptr<ItemInstance> item = craftSlots->removeItemNoUpdate(i);
        if (item != nullptr) {
            player->drop(item);
        }
    }
}

bool CraftingMenu::stillValid(std::shared_ptr<Player> player) {
    if (level->getTile(x, y, z) != Tile::workBench_Id) return false;
    if (player->distanceToSqr(x + 0.5, y + 0.5, z + 0.5) > 8 * 8) return false;
    return true;
}

std::shared_ptr<ItemInstance> CraftingMenu::quickMoveStack(
    std::shared_ptr<Player> player, int slotIndex) {
    std::shared_ptr<ItemInstance> clicked = nullptr;
    Slot* slot = slots.at(slotIndex);
    if (slot != nullptr && slot->hasItem()) {
        std::shared_ptr<ItemInstance> stack = slot->getItem();
        clicked = stack->copy();

        if (slotIndex == RESULT_SLOT) {
            if (!moveItemStackTo(stack, INV_SLOT_START, USE_ROW_SLOT_END,
                                 true)) {
                return nullptr;
            }
            slot->onQuickCraft(stack, clicked);
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
        } else {
            if (!moveItemStackTo(stack, INV_SLOT_START, USE_ROW_SLOT_END,
                                 false)) {
                return nullptr;
            }
        }
        if (stack->count == 0) {
            slot->set(nullptr);
        } else {
            slot->setChanged();
        }
        if (stack->count == clicked->count) {
            // nothing moved
            return nullptr;
        } else {
            slot->onTake(player, stack);
        }
    }
    return clicked;
}

bool CraftingMenu::canTakeItemForPickAll(std::shared_ptr<ItemInstance> carried,
                                         Slot* target) {
    return target->container != resultSlots &&
           AbstractContainerMenu::canTakeItemForPickAll(carried, target);
}