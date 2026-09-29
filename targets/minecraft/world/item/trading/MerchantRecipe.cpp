#include "MerchantRecipe.h"

#include <string>

#include "minecraft/world/item/ItemInstance.h"
#include "nbt/CompoundTag.h"

class Item;
class Tile;

void MerchantRecipe::_init(std::shared_ptr<ItemInstance> buyA,
                           std::shared_ptr<ItemInstance> buyB,
                           std::shared_ptr<ItemInstance> sell) {
    this->buyA = buyA;
    this->buyB = buyB;
    this->sell = sell;
    uses = 0;
    maxUses = 7;
}

MerchantRecipe::MerchantRecipe(CompoundTag* tag) {
    buyA = nullptr;
    buyB = nullptr;
    sell = nullptr;
    uses = 0;
    load(tag);
}

MerchantRecipe::MerchantRecipe(std::shared_ptr<ItemInstance> buyA,
                               std::shared_ptr<ItemInstance> buyB,
                               std::shared_ptr<ItemInstance> sell, int uses,
                               int maxUses) {
    _init(buyA, buyB, sell);
    this->uses = uses;
    this->maxUses = maxUses;
}

MerchantRecipe::MerchantRecipe(std::shared_ptr<ItemInstance> buy,
                               std::shared_ptr<ItemInstance> sell) {
    _init(buy, nullptr, sell);
}

MerchantRecipe::MerchantRecipe(std::shared_ptr<ItemInstance> buy, Item* sell) {
    _init(buy, nullptr, std::make_shared<ItemInstance>(sell));
}

MerchantRecipe::MerchantRecipe(std::shared_ptr<ItemInstance> buy, Tile* sell) {
    _init(buy, nullptr, std::make_shared<ItemInstance>(sell));
}

std::shared_ptr<ItemInstance> MerchantRecipe::getBuyAItem() { return buyA; }

std::shared_ptr<ItemInstance> MerchantRecipe::getBuyBItem() { return buyB; }

bool MerchantRecipe::hasSecondaryBuyItem() { return buyB != nullptr; }

std::shared_ptr<ItemInstance> MerchantRecipe::getSellItem() { return sell; }

bool MerchantRecipe::isSame(MerchantRecipe* other) {
    if (buyA->id != other->buyA->id || sell->id != other->sell->id) {
        return false;
    }
    return (buyB == nullptr && other->buyB == nullptr) ||
           (buyB != nullptr && other->buyB != nullptr &&
            buyB->id == other->buyB->id);
}

bool MerchantRecipe::isSameSameButBetter(MerchantRecipe* other) {
    // same deal, but cheaper
    return isSame(other) &&
           (buyA->count < other->buyA->count ||
            (buyB != nullptr && buyB->count < other->buyB->count));
}

int MerchantRecipe::getUses() { return uses; }

int MerchantRecipe::getMaxUses() { return maxUses; }

void MerchantRecipe::increaseUses() { uses++; }

void MerchantRecipe::increaseMaxUses(int amount) { maxUses += amount; }

bool MerchantRecipe::isDeprecated() { return uses >= maxUses; }

void MerchantRecipe::enforceDeprecated() { uses = maxUses; }

void MerchantRecipe::load(CompoundTag* tag) {
    CompoundTag* buyTag = tag->getCompound("buy");
    buyA = ItemInstance::fromTag(buyTag);
    CompoundTag* sellTag = tag->getCompound("sell");
    sell = ItemInstance::fromTag(sellTag);
    if (tag->contains("buyB")) {
        buyB = ItemInstance::fromTag(tag->getCompound("buyB"));
    }
    if (tag->contains("uses")) {
        uses = tag->getInt("uses");
    }
    if (tag->contains("maxUses")) {
        maxUses = tag->getInt("maxUses");
    } else {
        maxUses = 7;
    }
}

CompoundTag* MerchantRecipe::createTag() {
    CompoundTag* tag = new CompoundTag();
    tag->putCompound("buy", buyA->save(new CompoundTag("buy")));
    tag->putCompound("sell", sell->save(new CompoundTag("sell")));
    if (buyB != nullptr) {
        tag->putCompound("buyB", buyB->save(new CompoundTag("buyB")));
    }
    tag->putInt("uses", uses);
    tag->putInt("maxUses", maxUses);
    return tag;
}