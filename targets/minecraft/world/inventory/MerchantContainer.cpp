#include "MerchantContainer.h"

#include "MerchantMenu.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/trading/Merchant.h"
#include "minecraft/world/item/trading/MerchantRecipe.h"
#include "minecraft/world/item/trading/MerchantRecipeList.h"

MerchantContainer::~MerchantContainer() {}

MerchantContainer::MerchantContainer(std::shared_ptr<Player> player,
                                     std::shared_ptr<Merchant> villager) {
    this->player = player;
    merchant = villager;
    items = std::vector<std::shared_ptr<ItemInstance>>(3);
    items[0] = nullptr;
    items[1] = nullptr;
    items[2] = nullptr;
    activeRecipe = nullptr;
    selectionHint = 0;
}

unsigned int MerchantContainer::getContainerSize() { return items.size(); }

std::shared_ptr<ItemInstance> MerchantContainer::getItem(unsigned int slot) {
    return items[slot];
}

std::shared_ptr<ItemInstance> MerchantContainer::removeItem(unsigned int slot,
                                                            int count) {
    if (items[slot] != nullptr) {
        if (slot == MerchantMenu::RESULT_SLOT) {
            std::shared_ptr<ItemInstance> item = items[slot];
            items[slot] = nullptr;
            return item;
        }
        if (items[slot]->count <= count) {
            std::shared_ptr<ItemInstance> item = items[slot];
            items[slot] = nullptr;
            if (isPaymentSlot(slot)) {
                updateSellItem();
            }
            return item;
        } else {
            std::shared_ptr<ItemInstance> i = items[slot]->remove(count);
            if (items[slot]->count == 0) items[slot] = nullptr;
            if (isPaymentSlot(slot)) {
                updateSellItem();
            }
            return i;
        }
    }
    return nullptr;
}

bool MerchantContainer::isPaymentSlot(int slot) {
    return slot == MerchantMenu::PAYMENT1_SLOT ||
           slot == MerchantMenu::PAYMENT2_SLOT;
}

std::shared_ptr<ItemInstance> MerchantContainer::removeItemNoUpdate(int slot) {
    if (items[slot] != nullptr) {
        std::shared_ptr<ItemInstance> item = items[slot];
        items[slot] = nullptr;
        return item;
    }
    return nullptr;
}

void MerchantContainer::setItem(unsigned int slot,
                                std::shared_ptr<ItemInstance> item) {
    items[slot] = item;
    if (item != nullptr && item->count > getMaxStackSize())
        item->count = getMaxStackSize();
    if (isPaymentSlot(slot)) {
        updateSellItem();
    }
}

std::string MerchantContainer::getName() { return merchant->getDisplayName(); }

std::string MerchantContainer::getCustomName() { return ""; }

bool MerchantContainer::hasCustomName() { return false; }

int MerchantContainer::getMaxStackSize() {
    return Container::LARGE_MAX_STACK_SIZE;
}

bool MerchantContainer::stillValid(std::shared_ptr<Player> player) {
    return merchant->getTradingPlayer() == player;
}

void MerchantContainer::startOpen() {}

void MerchantContainer::stopOpen() {}

bool MerchantContainer::canPlaceItem(int slot,
                                     std::shared_ptr<ItemInstance> item) {
    return true;
}

void MerchantContainer::setChanged() { updateSellItem(); }

void MerchantContainer::updateSellItem() {
    activeRecipe = nullptr;

    std::shared_ptr<ItemInstance> buyItem1 = items[MerchantMenu::PAYMENT1_SLOT];
    std::shared_ptr<ItemInstance> buyItem2 = items[MerchantMenu::PAYMENT2_SLOT];

    if (buyItem1 == nullptr) {
        buyItem1 = buyItem2;
        buyItem2 = nullptr;
    }

    if (buyItem1 == nullptr) {
        setItem(MerchantMenu::RESULT_SLOT, nullptr);
    } else {
        MerchantRecipeList* offers = merchant->getOffers(player);
        if (offers != nullptr) {
            MerchantRecipe* recipeFor =
                offers->getRecipeFor(buyItem1, buyItem2, selectionHint);
            if (recipeFor != nullptr && !recipeFor->isDeprecated()) {
                activeRecipe = recipeFor;
                setItem(MerchantMenu::RESULT_SLOT,
                        recipeFor->getSellItem()->copy());
            } else if (buyItem2 != nullptr) {
                // try to switch
                recipeFor =
                    offers->getRecipeFor(buyItem2, buyItem1, selectionHint);
                if (recipeFor != nullptr && !recipeFor->isDeprecated()) {
                    activeRecipe = recipeFor;
                    setItem(MerchantMenu::RESULT_SLOT,
                            recipeFor->getSellItem()->copy());
                } else {
                    setItem(MerchantMenu::RESULT_SLOT, nullptr);
                }

            } else {
                setItem(MerchantMenu::RESULT_SLOT, nullptr);
            }
        }
    }

    merchant->notifyTradeUpdated(getItem(MerchantMenu::RESULT_SLOT));
}

MerchantRecipe* MerchantContainer::getActiveRecipe() { return activeRecipe; }

void MerchantContainer::setSelectionHint(int selectionHint) {
    this->selectionHint = selectionHint;
    updateSellItem();
}