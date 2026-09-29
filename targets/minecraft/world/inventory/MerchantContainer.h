#pragma once

#include <memory>
#include <string>
#include <vector>

#include "minecraft/world/Container.h"

class Merchant;
class Player;
class MerchantRecipe;
class ItemInstance;

class MerchantContainer : public Container {
private:
    std::shared_ptr<Merchant> merchant;
    std::vector<std::shared_ptr<ItemInstance>> items;
    std::shared_ptr<Player> player;
    MerchantRecipe* activeRecipe;
    int selectionHint;

public:
    MerchantContainer(std::shared_ptr<Player> player,
                      std::shared_ptr<Merchant> villager);
    ~MerchantContainer();

    unsigned int getContainerSize();
    std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    std::shared_ptr<ItemInstance> removeItem(unsigned int slot, int count);

private:
    bool isPaymentSlot(int slot);

public:
    std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    std::string getName();
    std::string getCustomName();
    bool hasCustomName();
    int getMaxStackSize();
    bool stillValid(std::shared_ptr<Player> player);
    void startOpen();
    void stopOpen();
    bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
    void setChanged();
    void updateSellItem();
    MerchantRecipe* getActiveRecipe();
    void setSelectionHint(int selectionHint);
};