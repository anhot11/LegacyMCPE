#pragma once

#include <memory>
#include <string>

class MerchantRecipeList;
class MerchantRecipe;
class Player;
class ItemInstance;

class Merchant {
public:
    virtual ~Merchant() {}
    virtual void setTradingPlayer(std::shared_ptr<Player> player) = 0;
    virtual std::shared_ptr<Player> getTradingPlayer() = 0;
    virtual MerchantRecipeList* getOffers(
        std::shared_ptr<Player> forPlayer) = 0;
    virtual void overrideOffers(MerchantRecipeList* recipeList) = 0;
    virtual void notifyTrade(MerchantRecipe* activeRecipe) = 0;
    virtual void notifyTradeUpdated(std::shared_ptr<ItemInstance> item) = 0;
    virtual std::string getDisplayName() = 0;
};