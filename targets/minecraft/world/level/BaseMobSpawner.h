#pragma once

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "minecraft/util/WeighedRandom.h"
#include "minecraft/world/entity/Entity.h"

class CompoundTag;
class Level;

class BaseMobSpawner {
public:
    class SpawnData : public WeighedRandomItem {
    public:
        CompoundTag* tag;
        std::string type;

        SpawnData(CompoundTag* base);
        SpawnData(CompoundTag* tag, std::string type);
        ~SpawnData();

        virtual CompoundTag* save();
    };

private:
    static const int EVENT_SPAWN = 1;

public:
    int spawnDelay;

private:
    std::string entityId;
    std::vector<SpawnData*>* spawnPotentials;
    SpawnData* nextSpawnData;

public:
    double spin, oSpin;

private:
    int minSpawnDelay;
    int maxSpawnDelay;
    int spawnCount;
    std::shared_ptr<Entity> displayEntity;
    int maxNearbyEntities;
    int requiredPlayerRange;
    int spawnRange;

public:
    BaseMobSpawner();
    virtual ~BaseMobSpawner();

    virtual std::string getEntityId();
    virtual void setEntityId(const std::string& entityId);
    virtual bool isNearPlayer();
    virtual void tick();
    virtual std::shared_ptr<Entity> loadDataAndAddEntity(
        std::shared_ptr<Entity> entity);

private:
    virtual void delay();

public:
    virtual void load(CompoundTag* tag);
    virtual void save(CompoundTag* tag);
    virtual std::shared_ptr<Entity> getDisplayEntity();
    virtual bool onEventTriggered(int id);
    virtual SpawnData* getNextSpawnData();
    virtual void setNextSpawnData(SpawnData* nextSpawnData);

    virtual void broadcastEvent(int id) = 0;
    virtual Level* getLevel() = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual int getZ() = 0;
};
