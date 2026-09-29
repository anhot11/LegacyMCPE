#include "AbstractContainerMenu.h"

#include <algorithm>

#include "Slot.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/entity/player/Abilities.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/inventory/net.minecraft.world.inventory.ContainerListener.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/redstone/Redstone.h"

// 4J Stu - The java does not have ctor here (being an abstract) but we need one
// to initialise the member variables
// TODO Make sure all derived classes also call this
AbstractContainerMenu::AbstractContainerMenu() {
    containerId = 0;

    changeUid = 0;

    quickcraftType = -1;
    quickcraftStatus = 0;

    m_bNeedsRendered = false;
}

AbstractContainerMenu::~AbstractContainerMenu() {
    for (unsigned int i = 0; i < slots.size(); i++) {
        delete slots.at(i);
    }
}

Slot* AbstractContainerMenu::addSlot(Slot* slot) {
    slot->index = (int)slots.size();
    slots.push_back(slot);
    lastSlots.push_back(nullptr);
    return slot;
}

void AbstractContainerMenu::addSlotListener(ContainerListener* listener) {
    containerListeners.push_back(listener);

    std::vector<std::shared_ptr<ItemInstance>>* items = getItems();
    listener->refreshContainer(this, items);
    delete items;
    broadcastChanges();
}

void AbstractContainerMenu::removeSlotListener(ContainerListener* listener) {
    auto it =
        find(containerListeners.begin(), containerListeners.end(), listener);
    if (it != containerListeners.end()) containerListeners.erase(it);
}

std::vector<std::shared_ptr<ItemInstance>>* AbstractContainerMenu::getItems() {
    std::vector<std::shared_ptr<ItemInstance>>* items =
        new std::vector<std::shared_ptr<ItemInstance>>();
    auto itEnd = slots.end();
    for (auto it = slots.begin(); it != itEnd; it++) {
        items->push_back((*it)->getItem());
    }
    return items;
}

void AbstractContainerMenu::sendData(int id, int value) {
    auto itEnd = containerListeners.end();
    for (auto it = containerListeners.begin(); it != itEnd; it++) {
        (*it)->setContainerData(this, id, value);
    }
}

void AbstractContainerMenu::broadcastChanges() {
    for (unsigned int i = 0; i < slots.size(); i++) {
        std::shared_ptr<ItemInstance> current = slots.at(i)->getItem();
        std::shared_ptr<ItemInstance> expected = lastSlots.at(i);
        if (!ItemInstance::matches(expected, current)) {
            // 4J Stu - Added 0 count check. There is a bug in the Java with
            // anvils that means this broadcast happens while we are in the
            // middle of quickmoving, and before the slot properly gets set to
            // null
            expected = (current == nullptr || current->count == 0)
                           ? nullptr
                           : current->copy();
            lastSlots[i] = expected;
            m_bNeedsRendered = true;

            auto itEnd = containerListeners.end();
            for (auto it = containerListeners.begin(); it != itEnd; it++) {
                (*it)->slotChanged(this, i, expected);
            }
        }
    }
}

bool AbstractContainerMenu::needsRendered() {
    bool needsRendered = m_bNeedsRendered;
    m_bNeedsRendered = false;

    for (unsigned int i = 0; i < slots.size(); i++) {
        std::shared_ptr<ItemInstance> current = slots.at(i)->getItem();
        std::shared_ptr<ItemInstance> expected = lastSlots.at(i);
        if (!ItemInstance::matches(expected, current)) {
            expected = current == nullptr ? nullptr : current->copy();
            lastSlots[i] = expected;
            needsRendered = true;
        }
    }

    return needsRendered;
}

bool AbstractContainerMenu::clickMenuButton(std::shared_ptr<Player> player,
                                            int buttonId) {
    return false;
}

Slot* AbstractContainerMenu::getSlotFor(std::shared_ptr<Container> c,
                                        int index) {
    auto itEnd = slots.end();
    for (auto it = slots.begin(); it != itEnd; it++) {
        Slot* slot = *it;  // slots->at(i);
        if (slot->isAt(c, index)) {
            return slot;
        }
    }
    return nullptr;
}

Slot* AbstractContainerMenu::getSlot(int index) { return slots.at(index); }

std::shared_ptr<ItemInstance> AbstractContainerMenu::quickMoveStack(
    std::shared_ptr<Player> player, int slotIndex) {
    Slot* slot = slots.at(slotIndex);
    if (slot != nullptr) {
        return slot->getItem();
    }
    return nullptr;
}

std::shared_ptr<ItemInstance> AbstractContainerMenu::clicked(
    int slotIndex, int buttonNum, int clickType, std::shared_ptr<Player> player,
    bool looped)  // 4J Added looped param
{
    std::shared_ptr<ItemInstance> clickedEntity = nullptr;
    std::shared_ptr<Inventory> inventory = player->inventory;

    if (clickType == CLICK_QUICK_CRAFT) {
        int expectedStatus = quickcraftStatus;
        quickcraftStatus = getQuickcraftHeader(buttonNum);

        if ((expectedStatus != QUICKCRAFT_HEADER_CONTINUE ||
             quickcraftStatus != QUICKCRAFT_HEADER_END) &&
            expectedStatus != quickcraftStatus) {
            resetQuickCraft();
        } else if (inventory->getCarried() == nullptr) {
            resetQuickCraft();
        } else if (quickcraftStatus == QUICKCRAFT_HEADER_START) {
            quickcraftType = getQuickcraftType(buttonNum);

            if (isValidQuickcraftType(quickcraftType)) {
                quickcraftStatus = QUICKCRAFT_HEADER_CONTINUE;
                quickcraftSlots.clear();
            } else {
                resetQuickCraft();
            }
        } else if (quickcraftStatus == QUICKCRAFT_HEADER_CONTINUE) {
            Slot* slot = slots.at(slotIndex);

            if (slot != nullptr &&
                canItemQuickReplace(slot, inventory->getCarried(), true) &&
                slot->mayPlace(inventory->getCarried()) &&
                inventory->getCarried()->count > quickcraftSlots.size() &&
                canDragTo(slot)) {
                quickcraftSlots.insert(slot);
            }
        } else if (quickcraftStatus == QUICKCRAFT_HEADER_END) {
            if (!quickcraftSlots.empty()) {
                std::shared_ptr<ItemInstance> source =
                    inventory->getCarried()->copy();
                int remaining = inventory->getCarried()->count;

                for (auto it = quickcraftSlots.begin();
                     it != quickcraftSlots.end(); ++it) {
                    Slot* slot = *it;
                    if (slot != nullptr &&
                        canItemQuickReplace(slot, inventory->getCarried(),
                                            true) &&
                        slot->mayPlace(inventory->getCarried()) &&
                        inventory->getCarried()->count >=
                            quickcraftSlots.size() &&
                        canDragTo(slot)) {
                        std::shared_ptr<ItemInstance> copy = source->copy();
                        int carry =
                            slot->hasItem() ? slot->getItem()->count : 0;
                        getQuickCraftSlotCount(&quickcraftSlots, quickcraftType,
                                               copy, carry);

                        if (copy->count > copy->getMaxStackSize())
                            copy->count = copy->getMaxStackSize();
                        if (copy->count > slot->getMaxStackSize())
                            copy->count = slot->getMaxStackSize();

                        remaining -= copy->count - carry;
                        slot->set(copy);
                    }
                }

                source->count = remaining;
                if (source->count <= 0) {
                    source = nullptr;
                }
                inventory->setCarried(source);
            }

            resetQuickCraft();
        } else {
            resetQuickCraft();
        }
    } else if (quickcraftStatus != QUICKCRAFT_HEADER_START) {
        resetQuickCraft();
    } else if ((clickType == CLICK_PICKUP || clickType == CLICK_QUICK_MOVE) &&
               (buttonNum == 0 || buttonNum == 1)) {
        if (slotIndex == SLOT_CLICKED_OUTSIDE) {
            if (inventory->getCarried() != nullptr) {
                if (slotIndex == SLOT_CLICKED_OUTSIDE) {
                    if (buttonNum == 0) {
                        player->drop(inventory->getCarried());
                        inventory->setCarried(nullptr);
                    }
                    if (buttonNum == 1) {
                        player->drop(inventory->getCarried()->remove(1));
                        if (inventory->getCarried()->count == 0)
                            inventory->setCarried(nullptr);
                    }
                }
            }
        } else if (clickType == CLICK_QUICK_MOVE) {
            if (slotIndex < 0) return nullptr;
            Slot* slot = slots.at(slotIndex);
            if (slot != nullptr && slot->mayPickup(player)) {
                std::shared_ptr<ItemInstance> piiClicked =
                    quickMoveStack(player, slotIndex);
                if (piiClicked != nullptr) {
                    int oldType = piiClicked->id;

                    // 4J Stu - We ignore the return value for loopClicks, so
                    // don't make a copy
                    if (!looped) {
                        clickedEntity = piiClicked->copy();
                    }

                    // 4J Stu - Remove the reference to this before we start a
                    // recursive loop
                    piiClicked = nullptr;

                    if (slot != nullptr) {
                        if (slot->getItem() != nullptr &&
                            slot->getItem()->id == oldType) {
                            if (looped) {
                                // Return a non-null value to indicate that we
                                // want to loop more
                                clickedEntity = std::shared_ptr<ItemInstance>(
                                    new ItemInstance(0, 1, 0));
                            } else {
                                // 4J Stu - Brought forward loopClick from 1.2
                                // to fix infinite recursion bug in creative
                                loopClick(slotIndex, buttonNum, true, player);
                            }
                        }
                    }
                }
            }
        } else {
            if (slotIndex < 0) return nullptr;

            Slot* slot = slots.at(slotIndex);
            if (slot != nullptr) {
                std::shared_ptr<ItemInstance> clicked = slot->getItem();
                std::shared_ptr<ItemInstance> carried = inventory->getCarried();

                if (clicked != nullptr) {
                    clickedEntity = clicked->copy();
                }

                if (clicked == nullptr) {
                    if (carried != nullptr && slot->mayPlace(carried)) {
                        int c = buttonNum == 0 ? carried->count : 1;
                        if (c > slot->getMaxStackSize()) {
                            c = slot->getMaxStackSize();
                        }
                        if (carried->count >= c) {
                            slot->set(carried->remove(c));
                        }
                        if (carried->count == 0) {
                            inventory->setCarried(nullptr);
                        }
                    }
                }
                // 4J Added for dyable armour and combinining damaged items
                else if (buttonNum == 1 && mayCombine(slot, carried)) {
                    std::shared_ptr<ItemInstance> combined =
                        slot->combine(carried);
                    if (combined != nullptr) {
                        slot->set(combined);
                        if (!player->abilities.instabuild) carried->remove(1);
                        if (carried->count == 0) {
                            inventory->setCarried(nullptr);
                        }
                    }
                } else if (slot->mayPickup(player)) {
                    if (carried == nullptr) {
                        // pick up to empty hand
                        int c = buttonNum == 0 ? clicked->count
                                               : (clicked->count + 1) / 2;
                        std::shared_ptr<ItemInstance> removed = slot->remove(c);

                        inventory->setCarried(removed);
                        if (clicked->count == 0) {
                            slot->set(nullptr);
                        }
                        slot->onTake(player, inventory->getCarried());
                    } else if (slot->mayPlace(carried)) {
                        // put down and/or pick up
                        if (clicked->id != carried->id ||
                            clicked->getAuxValue() != carried->getAuxValue() ||
                            !ItemInstance::tagMatches(clicked, carried)) {
                            // no match, replace
                            if (carried->count <= slot->getMaxStackSize()) {
                                slot->set(carried);
                                inventory->setCarried(clicked);
                            }
                        } else {
                            // match, attempt to fill slot
                            int c = buttonNum == 0 ? carried->count : 1;
                            if (c > slot->getMaxStackSize() - clicked->count) {
                                c = slot->getMaxStackSize() - clicked->count;
                            }
                            if (c >
                                carried->getMaxStackSize() - clicked->count) {
                                c = carried->getMaxStackSize() - clicked->count;
                            }
                            carried->remove(c);
                            if (carried->count == 0) {
                                inventory->setCarried(nullptr);
                            }
                            clicked->count += c;
                        }
                    } else {
                        // pick up to non-empty hand
                        if (clicked->id == carried->id &&
                            carried->getMaxStackSize() > 1 &&
                            (!clicked->isStackedByData() ||
                             clicked->getAuxValue() ==
                                 carried->getAuxValue()) &&
                            ItemInstance::tagMatches(clicked, carried)) {
                            int c = clicked->count;
                            if (c > 0 && c + carried->count <=
                                             carried->getMaxStackSize()) {
                                carried->count += c;
                                clicked = slot->remove(c);
                                if (clicked->count == 0) slot->set(nullptr);
                                slot->onTake(player, inventory->getCarried());
                            }
                        }
                    }
                }
                slot->setChanged();
            }
        }
    } else if (clickType == CLICK_SWAP && buttonNum >= 0 && buttonNum < 9) {
        Slot* slot = slots.at(slotIndex);
        if (slot->mayPickup(player)) {
            std::shared_ptr<ItemInstance> current =
                inventory->getItem(buttonNum);
            bool canMove =
                current == nullptr ||
                (slot->container == inventory && slot->mayPlace(current));
            int freeSlot = -1;

            if (!canMove) {
                freeSlot = inventory->getFreeSlot();
                canMove |= freeSlot > -1;
            }

            if (slot->hasItem() && canMove) {
                std::shared_ptr<ItemInstance> taking = slot->getItem();
                inventory->setItem(buttonNum, taking);

                if ((slot->container == inventory && slot->mayPlace(current)) ||
                    current == nullptr) {
                    slot->remove(taking->count);
                    slot->set(current);
                    slot->onTake(player, taking);
                } else if (freeSlot > -1) {
                    inventory->add(current);
                    slot->remove(taking->count);
                    slot->set(nullptr);
                    slot->onTake(player, taking);
                }
            } else if (!slot->hasItem() && current != nullptr &&
                       slot->mayPlace(current)) {
                inventory->setItem(buttonNum, nullptr);
                slot->set(current);
            }
        }
    } else if (clickType == CLICK_CLONE && player->abilities.instabuild &&
               inventory->getCarried() == nullptr && slotIndex >= 0) {
        Slot* slot = slots.at(slotIndex);
        if (slot != nullptr && slot->hasItem()) {
            std::shared_ptr<ItemInstance> copy = slot->getItem()->copy();
            copy->count = copy->getMaxStackSize();
            inventory->setCarried(copy);
        }
    } else if (clickType == CLICK_THROW && inventory->getCarried() == nullptr &&
               slotIndex >= 0) {
        Slot* slot = slots.at(slotIndex);
        if (slot != nullptr && slot->hasItem() && slot->mayPickup(player)) {
            std::shared_ptr<ItemInstance> item =
                slot->remove(buttonNum == 0 ? 1 : slot->getItem()->count);
            slot->onTake(player, item);
            player->drop(item);
        }
    } else if (clickType == CLICK_PICKUP_ALL && slotIndex >= 0) {
        Slot* slot = slots.at(slotIndex);
        std::shared_ptr<ItemInstance> carried = inventory->getCarried();

        if (carried != nullptr &&
            (slot == nullptr || !slot->hasItem() || !slot->mayPickup(player))) {
            int start = buttonNum == 0 ? 0 : slots.size() - 1;
            int step = buttonNum == 0 ? 1 : -1;

            for (int pass = 0; pass < 2; pass++) {
                // In the first pass, we only get partial stacks.
                for (int i = start; i >= 0 && i < slots.size() &&
                                    carried->count < carried->getMaxStackSize();
                     i += step) {
                    Slot* target = slots.at(i);

                    if (target->hasItem() &&
                        canItemQuickReplace(target, carried, true) &&
                        target->mayPickup(player) &&
                        canTakeItemForPickAll(carried, target)) {
                        if (pass == 0 &&
                            target->getItem()->count ==
                                target->getItem()->getMaxStackSize())
                            continue;
                        int count = std::min(
                            carried->getMaxStackSize() - carried->count,
                            target->getItem()->count);
                        std::shared_ptr<ItemInstance> removed =
                            target->remove(count);
                        carried->count += count;

                        if (removed->count <= 0) {
                            target->set(nullptr);
                        }
                        target->onTake(player, removed);
                    }
                }
            }
        }

        broadcastChanges();
    }
    return clickedEntity;
}

bool AbstractContainerMenu::canTakeItemForPickAll(
    std::shared_ptr<ItemInstance> carried, Slot* target) {
    return true;
}

// 4J Stu - Brought forward from 1.2 to fix infinite recursion bug in creative
void AbstractContainerMenu::loopClick(int slotIndex, int buttonNum,
                                      bool quickKeyHeld,
                                      std::shared_ptr<Player> player) {
    while (clicked(slotIndex, buttonNum, CLICK_QUICK_MOVE, player, true) !=
           nullptr) {
    }
}

bool AbstractContainerMenu::mayCombine(Slot* slot,
                                       std::shared_ptr<ItemInstance> item) {
    return false;
}

void AbstractContainerMenu::removed(std::shared_ptr<Player> player) {
    std::shared_ptr<Inventory> inventory = player->inventory;
    if (inventory->getCarried() != nullptr) {
        player->drop(inventory->getCarried());
        inventory->setCarried(nullptr);
    }
}

void AbstractContainerMenu::
    slotsChanged()  // 4J used to take a shared_ptr<Container> but wasn't using
                    // it, so removed to simplify things
{
    broadcastChanges();
}

bool AbstractContainerMenu::isPauseScreen() { return false; }

void AbstractContainerMenu::setItem(unsigned int slot,
                                    std::shared_ptr<ItemInstance> item) {
    getSlot(slot)->set(item);
}

void AbstractContainerMenu::setAll(
    std::vector<std::shared_ptr<ItemInstance>>* items) {
    for (unsigned int i = 0; i < items->size(); i++) {
        getSlot(i)->set((*items)[i]);
    }
}

void AbstractContainerMenu::setData(int id, int value) {}

short AbstractContainerMenu::backup(std::shared_ptr<Inventory> inventory) {
    changeUid++;
    return changeUid;
}

bool AbstractContainerMenu::isSynched(std::shared_ptr<Player> player) {
    return !(unSynchedPlayers.find(player) != unSynchedPlayers.end());
}

void AbstractContainerMenu::setSynched(std::shared_ptr<Player> player,
                                       bool synched) {
    if (synched) {
        auto it = unSynchedPlayers.find(player);

        if (it != unSynchedPlayers.end()) unSynchedPlayers.erase(it);
    } else {
        unSynchedPlayers.insert(player);
    }
}

// 4J Stu - Brought a few changes in this function forward from 1.2 to make it
// return a bool
bool AbstractContainerMenu::moveItemStackTo(
    std::shared_ptr<ItemInstance> itemStack, int startSlot, int endSlot,
    bool backwards) {
    bool anythingChanged = false;

    int destSlot = startSlot;
    if (backwards) {
        destSlot = endSlot - 1;
    }

    // find stackable slots first
    if (itemStack->isStackable()) {
        while (itemStack->count > 0 && ((!backwards && destSlot < endSlot) ||
                                        (backwards && destSlot >= startSlot))) {
            Slot* slot = slots.at(destSlot);
            std::shared_ptr<ItemInstance> target = slot->getItem();
            if (target != nullptr && target->id == itemStack->id &&
                (!itemStack->isStackedByData() ||
                 itemStack->getAuxValue() == target->getAuxValue()) &&
                ItemInstance::tagMatches(itemStack, target)) {
                int totalStack = target->count + itemStack->count;
                if (totalStack <= itemStack->getMaxStackSize()) {
                    itemStack->count = 0;
                    target->count = totalStack;
                    slot->setChanged();
                    anythingChanged = true;
                } else if (target->count < itemStack->getMaxStackSize()) {
                    itemStack->count -=
                        (itemStack->getMaxStackSize() - target->count);
                    target->count = itemStack->getMaxStackSize();
                    slot->setChanged();
                    anythingChanged = true;
                }
            }

            if (backwards) {
                destSlot--;
            } else {
                destSlot++;
            }
        }
    }

    // find empty slot
    if (itemStack->count > 0) {
        if (backwards) {
            destSlot = endSlot - 1;
        } else {
            destSlot = startSlot;
        }
        while ((!backwards && destSlot < endSlot) ||
               (backwards && destSlot >= startSlot)) {
            Slot* slot = slots.at(destSlot);
            std::shared_ptr<ItemInstance> target = slot->getItem();

            if (target == nullptr) {
                slot->set(itemStack->copy());
                slot->setChanged();
                itemStack->count = 0;
                anythingChanged = true;
                break;
            }

            if (backwards) {
                destSlot--;
            } else {
                destSlot++;
            }
        }
    }
    return anythingChanged;
}

bool AbstractContainerMenu::isOverrideResultClick(int slotNum, int buttonNum) {
    return false;
}

int AbstractContainerMenu::getQuickcraftType(int mask) {
    return (mask >> 2) & 0x3;
}

int AbstractContainerMenu::getQuickcraftHeader(int mask) { return mask & 0x3; }

int AbstractContainerMenu::getQuickcraftMask(int header, int type) {
    return (header & 0x3) | ((type & 0x3) << 2);
}

bool AbstractContainerMenu::isValidQuickcraftType(int type) {
    return type == QUICKCRAFT_TYPE_CHARITABLE || type == QUICKCRAFT_TYPE_GREEDY;
}

void AbstractContainerMenu::resetQuickCraft() {
    quickcraftStatus = QUICKCRAFT_HEADER_START;
    quickcraftSlots.clear();
}

bool AbstractContainerMenu::canItemQuickReplace(
    Slot* slot, std::shared_ptr<ItemInstance> item, bool ignoreSize) {
    bool canReplace = slot == nullptr || !slot->hasItem();

    if (slot != nullptr && slot->hasItem() && item != nullptr &&
        item->sameItem(slot->getItem()) &&
        ItemInstance::tagMatches(slot->getItem(), item)) {
        canReplace |= slot->getItem()->count + (ignoreSize ? 0 : item->count) <=
                      item->getMaxStackSize();
    }

    return canReplace;
}

void AbstractContainerMenu::getQuickCraftSlotCount(
    std::unordered_set<Slot*>* quickCraftSlots, int quickCraftingType,
    std::shared_ptr<ItemInstance> item, int carry) {
    switch (quickCraftingType) {
        case QUICKCRAFT_TYPE_CHARITABLE:
            item->count =
                Mth::floor(item->count / (float)quickCraftSlots->size());
            break;
        case QUICKCRAFT_TYPE_GREEDY:
            item->count = 1;
            break;
    }

    item->count += carry;
}

bool AbstractContainerMenu::canDragTo(Slot* slot) { return true; }

int AbstractContainerMenu::getRedstoneSignalFromContainer(
    std::shared_ptr<Container> container) {
    if (container == nullptr) return 0;
    int count = 0;
    float totalPct = 0;

    for (int i = 0; i < container->getContainerSize(); i++) {
        std::shared_ptr<ItemInstance> item = container->getItem(i);

        if (item != nullptr) {
            totalPct +=
                item->count / (float)std::min(container->getMaxStackSize(),
                                              item->getMaxStackSize());
            count++;
        }
    }

    totalPct /= container->getContainerSize();
    return Mth::floor(totalPct * (Redstone::SIGNAL_MAX - 1)) +
           (count > 0 ? 1 : 0);
}

// 4J Added
bool AbstractContainerMenu::isValidIngredient(
    std::shared_ptr<ItemInstance> item, int slotId) {
    return true;
}