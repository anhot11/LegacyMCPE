#include "ClientSideMerchant.h"

#include "minecraft/world/inventory/MerchantContainer.h"
#include "minecraft/world/item/trading/MerchantRecipe.h"
#include "minecraft/world/item/trading/MerchantRecipeList.h"

class Container;
class Player;

ClientSideMerchant::ClientSideMerchant(std::shared_ptr<Player> source,
                                       const std::string& name) {
    this->source = source;
    // 4J Stu - Need to do this after creating as a shared_ptr
    container = nullptr;  // new MerchantContainer(source, this);
    currentOffers = nullptr;
    m_name = name;
}

ClientSideMerchant::~ClientSideMerchant() {
    delete container;
    delete currentOffers;
}

void ClientSideMerchant::createContainer() {
    container = new MerchantContainer(source, shared_from_this());
}

Container* ClientSideMerchant::getContainer() { return container; }

std::shared_ptr<Player> ClientSideMerchant::getTradingPlayer() {
    return source;
}

void ClientSideMerchant::setTradingPlayer(std::shared_ptr<Player> player) {}

MerchantRecipeList* ClientSideMerchant::getOffers(
    std::shared_ptr<Player> forPlayer) {
    return currentOffers;
}

void ClientSideMerchant::overrideOffers(MerchantRecipeList* recipeList) {
    delete currentOffers;
    currentOffers = recipeList;
}

void ClientSideMerchant::notifyTrade(MerchantRecipe* activeRecipe) {
    activeRecipe->increaseUses();
}

void ClientSideMerchant::notifyTradeUpdated(
    std::shared_ptr<ItemInstance> item) {}

std::string ClientSideMerchant::getDisplayName() { return m_name; }