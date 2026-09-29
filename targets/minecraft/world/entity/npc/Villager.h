#pragma once

#include <stdint.h>

#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>

#include "Npc.h"
#include "java/Class.h"
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/world/entity/AgeableMob.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/trading/Merchant.h"

class Level;
class Village;
class MerchantRecipeList;
class MerchantRecipe;
class Entity;
class Player;
class Random;

class Villager : public AgableMob, public Npc, public Merchant {
public:
    eINSTANCEOF GetType() { return eTYPE_VILLAGER; }
    static Entity* create(Level* level) { return new Villager(level); }

    // public static final String comment = "No, I won't 'fix' these! They're
    // fine!! - Notch";

public:
    static const int PROFESSION_FARMER = 0;
    static const int PROFESSION_LIBRARIAN = 1;
    static const int PROFESSION_PRIEST = 2;
    static const int PROFESSION_SMITH = 3;
    static const int PROFESSION_BUTCHER = 4;
    static const int PROFESSION_MAX = 5;

private:
    static const int DATA_PROFESSION_ID = 16;
    int villageUpdateInterval;

    bool inLove;
    bool chasing;
    std::weak_ptr<Village> village;

    std::weak_ptr<Player> tradingPlayer;
    MerchantRecipeList* offers;
    int updateMerchantTimer;
    bool addRecipeOnUpdate;
    int riches;
    std::string lastPlayerTradeName;

    bool rewardPlayersOnFirstVillage;

private:
    void _init(int profession);

public:
    Villager(Level* level);
    Villager(Level* level, int profession);
    ~Villager();

protected:
    virtual void registerAttributes();

public:
    virtual bool useNewAi();

protected:
    virtual void serverAiMobStep();

public:
    virtual bool mobInteract(std::shared_ptr<Player> player);

protected:
    virtual void defineSynchedData();

public:
    virtual void addAdditonalSaveData(CompoundTag* tag);
    virtual void readAdditionalSaveData(CompoundTag* tag);

protected:
    virtual bool removeWhenFarAway();
    virtual int getAmbientSound();
    virtual int getHurtSound();
    virtual int getDeathSound();

public:
    void setProfession(int profession);
    int getProfession();
    bool isInLove();
    void setInLove(bool inLove);
    void setChasing(bool chasing);
    bool isChasing();
    void setLastHurtByMob(std::shared_ptr<LivingEntity> mob);
    void die(DamageSource* source);

    void handleEntityEvent(uint8_t id);

private:
    void addParticlesAroundSelf(ePARTICLE_TYPE particle);

public:
    void setTradingPlayer(std::shared_ptr<Player> player);
    std::shared_ptr<Player> getTradingPlayer();
    bool isTrading();
    void notifyTrade(MerchantRecipe* activeRecipe);
    void notifyTradeUpdated(std::shared_ptr<ItemInstance> item);
    MerchantRecipeList* getOffers(std::shared_ptr<Player> forPlayer);

private:
    float baseRecipeChanceMod;

    float getRecipeChance(float baseChance);
    void addOffers(int addCount);

public:
    void overrideOffers(MerchantRecipeList* recipeList);

private:
    static std::unordered_map<int, std::pair<int, int> > MIN_MAX_VALUES;
    static std::unordered_map<int, std::pair<int, int> > MIN_MAX_PRICES;

public:
    static void staticCtor();

private:
    /**
     * Adds a merchant recipe that trades items for a single ruby.
     *
     * @param list
     * @param itemId
     * @param random
     * @param likelyHood
     */
    static void addItemForTradeIn(MerchantRecipeList* list, int itemId,
                                  Random* random, float likelyHood);
    static std::shared_ptr<ItemInstance> getItemTradeInValue(int itemId,
                                                             Random* random);
    static int getTradeInValue(int itemId, Random* random);

    /**
     * Adds a merchant recipe that trades rubies for an item. If the cost is
     * negative, one ruby will give several of that item.
     *
     * @param list
     * @param itemId
     * @param random
     * @param likelyHood
     */
    static void addItemForPurchase(MerchantRecipeList* list, int itemId,
                                   Random* random, float likelyHood);
    static int getPurchaseCost(int itemId, Random* random);

public:
    virtual MobGroupData* finalizeMobSpawn(
        MobGroupData* groupData,
        int extraData = 0);  // 4J Added extraData param
    virtual void setRewardPlayersInVillage();
    virtual std::shared_ptr<AgableMob> getBreedOffspring(
        std::shared_ptr<AgableMob> target);
    virtual bool canBeLeashed();
    virtual std::string getDisplayName();
};