#pragma once

#include <format>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/ai/village/DoorInfo.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/phys/Vec3.h"

class CompoundTag;
class Level;
class Pos;

class Village {
private:
    Level* level;
    std::vector<std::shared_ptr<DoorInfo> > doorInfos;

    Pos* accCenter;
    Pos* center;
    int radius;
    int stableSince;
    int _tick;
    int populationSize;
    int noBreedTimer;

    std::unordered_map<std::string, int> playerStanding;

    class Aggressor {
    public:
        std::shared_ptr<LivingEntity> mob;
        int timeStamp;

        Aggressor(std::shared_ptr<LivingEntity> mob, int timeStamp);
    };

    std::vector<Aggressor*> aggressors;
    int golemCount;

public:
    Village();
    Village(Level* level);
    ~Village();

    void setLevel(Level* level);

    void tick(int tick);

private:
    std::optional<Vec3> findRandomSpawnPos(int x, int y, int z, int sx, int sy,
                                           int sz);
    bool canSpawnAt(int x, int y, int z, int sx, int sy, int sz);
    void countGolem();
    void countPopulation();

public:
    Pos* getCenter();
    int getRadius();
    int getDoorCount();
    int getStableAge();
    int getPopulationSize();
    bool isInside(int xx, int yy, int zz);
    std::vector<std::shared_ptr<DoorInfo> >* getDoorInfos();
    std::shared_ptr<DoorInfo> getClosestDoorInfo(int x, int y, int z);
    std::shared_ptr<DoorInfo> getBestDoorInfo(int x, int y, int z);
    bool hasDoorInfo(int x, int y, int z);
    std::shared_ptr<DoorInfo> getDoorInfo(int x, int y, int z);
    void addDoorInfo(std::shared_ptr<DoorInfo> di);
    bool canRemove();
    void addAggressor(std::shared_ptr<LivingEntity> mob);
    std::shared_ptr<LivingEntity> getClosestAggressor(
        std::shared_ptr<LivingEntity> from);
    std::shared_ptr<Player> getClosestBadStandingPlayer(
        std::shared_ptr<LivingEntity> from);

private:
    void updateAggressors();
    void updateDoors();
    bool isDoor(int x, int y, int z);
    void calcInfo();

public:
    int getStanding(const std::string& playerName);
    int modifyStanding(const std::string& playerName, int delta);
    bool isGoodStanding(const std::string& playerName);
    bool isBadStanding(const std::string& playerName);
    bool isVeryBadStanding(const std::string playerName);
    void readAdditionalSaveData(CompoundTag* tag);
    void addAdditonalSaveData(CompoundTag* tag);
    void resetNoBreedTimer();
    bool isBreedTimerOk();
    void rewardAllPlayers(int amount);
};
