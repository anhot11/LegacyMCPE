#pragma once

#include <memory>

#include "Slot.h"

class MerchantContainer;
class Player;
class Merchant;
class ItemInstance;
class MerchantRecipe;

class MerchantResultSlot : public Slot {
private:
    std::shared_ptr<MerchantContainer> slots;
    Player* player;
    int removeCount;
    std::shared_ptr<Merchant> merchant;

public:
    MerchantResultSlot(Player* player, std::shared_ptr<Merchant> merchant,
                       std::shared_ptr<MerchantContainer> slots, int id, int x,
                       int y);

    bool mayPlace(std::shared_ptr<ItemInstance> item);
    std::shared_ptr<ItemInstance> remove(int c);

protected:
    void onQuickCraft(std::shared_ptr<ItemInstance> picked, int count);
    void checkTakeAchievements(std::shared_ptr<ItemInstance> carried);

public:
    void onTake(std::shared_ptr<Player> player,
                std::shared_ptr<ItemInstance> carried);
    virtual bool mayCombine(std::shared_ptr<ItemInstance> item);  // 4J Added

private:
    bool removePaymentItemsIfMatching(MerchantRecipe* activeRecipe,
                                      std::shared_ptr<ItemInstance> a,
                                      std::shared_ptr<ItemInstance> b);
};