#pragma once

#include "GeneralStat.h"
#include "GenericStats.h"

enum ELeaderboardId {
    eLeaderboardId_TRAVELLING = 0,
    eLeaderboardId_MINING = 1,
    eLeaderboardId_FARMING = 2,
    eLeaderboardId_KILLING = 3
};

class DsItemEvent : public Stat {
public:
    static std::string nameMethods[];

    enum eAcquisitionMethod {
        eAcquisitionMethod_None = 0,

        eAcquisitionMethod_Pickedup,
        eAcquisitionMethod_Crafted,
        eAcquisitionMethod_TakenFromChest,
        eAcquisitionMethod_TakenFromEnderchest,
        eAcquisitionMethod_Bought,
        eAcquisitionMethod_Smithed,
        eAcquisitionMethod_Mined,

        eAcquisitionMethod_Placed,

        eAcquisitionMethod_MAX
    };

    typedef struct _Param {
        int methodId, itemId, itemAux, itemCount;
    } Param;

    DsItemEvent(int id, const std::string& name);

    bool onLeaderboard(ELeaderboardId leaderboard, eAcquisitionMethod methodId,
                       Param* param);
    int mergeIds(int itemId);

    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& param);
    static std::vector<uint8_t> createParamBlob(eAcquisitionMethod methodId,
                                                int itemId, int itemAux,
                                                int itemCount);
};

class DsMobKilled : public Stat {
public:
    static const bool RANGED = true;
    static const bool MELEE = false;

    static const int SPIDER_JOCKEY_ID = 49;

    DsMobKilled(int id, const std::string& name);

    typedef struct {
        bool isRanged;
        int mobType, weaponId, distance, damage;
    } Param;
    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& param);
    static std::vector<uint8_t> createParamBlob(std::shared_ptr<Player> plr,
                                                std::shared_ptr<Mob> mob,
                                                DamageSource* dmgSrc);
};

class DsMobInteract : public Stat {
protected:
    static std::string nameInteract[];

public:
    enum eInteract {
        eInteract_None = 0,
        eInteract_Breed,
        eInteract_Tamed,
        eInteract_Cured,
        eInteract_Crafted,
        eInteract_Sheared
    };

    DsMobInteract(int id, const std::string& name);

    typedef struct {
        int interactionType, mobId;
    } Param;
    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& param);
    static std::vector<uint8_t> createParamBlob(eInteract interactionId,
                                                int entityId);
};

class DsTravel : public Stat {
public:
    static std::string nameMethods[];

    enum eMethod {
        eMethod_walk,
        eMethod_swim,
        eMethod_fall,
        eMethod_climb,
        eMethod_minecart,
        eMethod_boat,
        eMethod_pig,

        eMethod_time,  // Time is a dimension too right...

        eMethod_MAX
    };

    static unsigned int CACHE_SIZES[eMethod_MAX];

    DsTravel(int id, const std::string& name);

    typedef struct {
        eMethod method;
        int distance;
    } Param;
    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& paramBlob);
    static std::vector<uint8_t> createParamBlob(eMethod method, int distance);

    void flush(std::shared_ptr<LocalPlayer> plr);

protected:
    unsigned int param_cache[MAX_LOCAL_PLAYERS][eMethod_MAX];
    int cache(int iPad, Param& param);
    void write(std::shared_ptr<LocalPlayer> plr, eMethod method, int distance);
};

class DsItemUsed : public Stat {
public:
    DsItemUsed(int id, const std::string& name);
    typedef struct {
        int itemId, aux, count, health, hunger;
    } Param;
    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& paramBlob);
    static std::vector<uint8_t> createParamBlob(int itemId, int aux, int count,
                                                int health, int hunger);
};

class DsAchievement : public Stat {
public:
    DsAchievement(int id, const std::string& name);

    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& paramBlob);

    typedef struct {
        eAward award;
    } SmallParam;
    static std::vector<uint8_t> createSmallParamBlob(eAward id);

    typedef struct {
        eAward award;
        int count;
    } LargeParam;
    static std::vector<uint8_t> createLargeParamBlob(eAward id, int count);
};

class DsChangedDimension : public Stat {
public:
    DsChangedDimension(int id, const std::string& name);
    typedef struct {
        int fromDimId, toDimId;
    } Param;
    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& paramBlob);
    static std::vector<uint8_t> createParamBlob(int fromDimId, int toDimId);
};

class DsEnteredBiome : public Stat {
public:
    DsEnteredBiome(int id, const std::string& name);
    typedef struct {
        int biomeId;
    } Param;
    virtual void handleParamBlob(std::shared_ptr<LocalPlayer> plr,
                                 std::vector<uint8_t>& paramBlob);
    static std::vector<uint8_t> createParamBlob(int biomeId);
};

class DurangoStats : public GenericStats {
public:
    static DurangoStats* getInstance() {
        return (DurangoStats*)GenericStats::getInstance();
    }

protected:
    enum {
        itemsAcquired_Id = 1,
        itemUsed_Id,
        travel_Id,
        mobKilled_Id,
        mobInteract_Id,
        binAchievement_Id,
        binAchievementLocal_Id,
        changedDimension_Id,
        enteredBiome_Id,
    };

    DsItemEvent* itemsAcquired;
    DsTravel* travel;

    DsMobKilled* mobKilled;
    DsMobInteract* mobInteract;

    DsAchievement* achievement;
    DsAchievement* achievementLocal;

    DsItemUsed* itemUsed;

    DsChangedDimension* changedDimension;
    DsEnteredBiome* enteredBiome;

    GUID playerSessionId;
    Platform::String ^ multiplayerCorrelationId;

public:
    DurangoStats();
    ~DurangoStats();

    virtual Stat* get_stat(int i);

protected:
    // Stats

    virtual Stat* get_walkOneM();
    virtual Stat* get_swimOneM();
    virtual Stat* get_fallOneM();
    virtual Stat* get_climbOneM();
    virtual Stat* get_minecartOneM();
    virtual Stat* get_boatOneM();
    virtual Stat* get_pigOneM();

    virtual Stat* get_cowsMilked();

    // Kills.
    virtual Stat* get_killMob();

    // Mob-Interactions.
    virtual Stat* get_breedEntity(eINSTANCEOF entityId);
    virtual Stat* get_tamedEntity(eINSTANCEOF entityId);
    virtual Stat* get_curedEntity(eINSTANCEOF entityId);
    virtual Stat* get_craftedEntity(eINSTANCEOF entityId);
    virtual Stat* get_shearedEntity(eINSTANCEOF entityId);

    virtual Stat* get_timePlayed();

    virtual Stat* get_blocksPlaced(int blockId);
    virtual Stat* get_blocksMined(int blockId);
    virtual Stat* get_itemsCollected(int itemId, int itemAux);
    virtual Stat* get_itemsCrafted(int itemId);
    virtual Stat* get_itemsSmelted(int itemId);
    virtual Stat* get_itemsUsed(int itemId);
    virtual Stat* get_itemsBought(int itemId);

    virtual Stat* get_changedDimension(int from, int to);
    virtual Stat* get_enteredBiome(int biomeId);

    // Achievements

    virtual Stat* get_achievement(eAward achievementId);

    // Parameters

    virtual std::vector<uint8_t> getParam_walkOneM(int distance);
    virtual std::vector<uint8_t> getParam_swimOneM(int distance);
    virtual std::vector<uint8_t> getParam_fallOneM(int distance);
    virtual std::vector<uint8_t> getParam_climbOneM(int distance);
    virtual std::vector<uint8_t> getParam_minecartOneM(int distance);
    virtual std::vector<uint8_t> getParam_boatOneM(int distance);
    virtual std::vector<uint8_t> getParam_pigOneM(int distance);

    virtual std::vector<uint8_t> getParam_cowsMilked();

    virtual std::vector<uint8_t> getParam_blocksPlaced(int blockId, int data,
                                                       int count);
    virtual std::vector<uint8_t> getParam_blocksMined(int blockId, int data,
                                                      int count);
    virtual std::vector<uint8_t> getParam_itemsCollected(int id, int aux,
                                                         int count);
    virtual std::vector<uint8_t> getParam_itemsCrafted(int id, int aux,
                                                       int count);
    virtual std::vector<uint8_t> getParam_itemsUsed(
        std::shared_ptr<Player> plr, std::shared_ptr<ItemInstance> itm);
    virtual std::vector<uint8_t> getParam_itemsBought(int id, int aux,
                                                      int count);

    virtual std::vector<uint8_t> getParam_mobKill(std::shared_ptr<Player> plr,
                                                  std::shared_ptr<Mob> mob,
                                                  DamageSource* dmgSrc);

    virtual std::vector<uint8_t> getParam_breedEntity(eINSTANCEOF entityId);
    virtual std::vector<uint8_t> getParam_tamedEntity(eINSTANCEOF entityId);
    virtual std::vector<uint8_t> getParam_curedEntity(eINSTANCEOF entityId);
    virtual std::vector<uint8_t> getParam_craftedEntity(eINSTANCEOF entityId);
    virtual std::vector<uint8_t> getParam_shearedEntity(eINSTANCEOF entityId);

    virtual std::vector<uint8_t> getParam_time(int timediff);

    virtual std::vector<uint8_t> getParam_changedDimension(int from, int to);
    virtual std::vector<uint8_t> getParam_enteredBiome(int biomeId);

    virtual std::vector<uint8_t> getParam_achievement(eAward id);

    virtual std::vector<uint8_t> getParam_onARail(int dist);
    virtual std::vector<uint8_t> getParam_chestfulOfCobblestone(int count);
    // virtual std::vector<uint8_t> getParam_openInventory(
    virtual std::vector<uint8_t> getParam_overkill(int dmg);
    virtual std::vector<uint8_t> getParam_musicToMyEars(int recordId);

    // Helpers

public:
    // Achievements that have no parameters, you have earned them or not.
    static bool binaryAchievement(eAward achievementId);

    // Achievements that have parameters, but the event is specifically for this
    // achievement.
    static bool enhancedAchievement(eAward achievementId);

    static void generatePlayerSession();
    static LPCGUID getPlayerSession();

    static void setMultiplayerCorrelationId(Platform::String ^ mpcId);
    static const char* getMultiplayerCorrelationId();

    static const char* getUserId(std::shared_ptr<LocalPlayer> plr);
    static const char* getUserId(int iPad);

    static void playerSessionStart(PlayerUID, std::shared_ptr<Player>);
    static void playerSessionStart(int iPad);
    static void playerSessionPause(int iPad);
    static void playerSessionResume(int iPad);
    static void playerSessionEnd(int iPad);
};