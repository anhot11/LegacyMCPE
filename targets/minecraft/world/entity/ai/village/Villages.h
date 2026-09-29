#pragma once

#include <deque>
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "minecraft/world/entity/ai/village/DoorInfo.h"
#include "minecraft/world/entity/ai/village/Village.h"
#include "minecraft/world/level/saveddata/SavedData.h"

class Level;
class Pos;

class Villages : public SavedData {
public:
    static const std::string VILLAGE_FILE_ID;

    static const int MaxDoorDist = 32;

private:
    Level* level;
    std::deque<Pos*> queries;
    std::vector<std::shared_ptr<DoorInfo> > unclustered;
    std::vector<std::shared_ptr<Village> > villages;
    int _tick;

public:
    Villages(const std::string& id);
    Villages(Level* level);
    ~Villages();

    void setLevel(Level* level);
    void queryUpdateAround(int x, int y, int z);
    void tick();

private:
    void removeVillages();

public:
    std::vector<std::shared_ptr<Village> >* getVillages();
    std::shared_ptr<Village> getClosestVillage(int x, int y, int z,
                                               int maxDist);

private:
    void processNextQuery();
    void cluster();
    void addDoorInfos(Pos* pos);
    std::shared_ptr<DoorInfo> getDoorInfo(int x, int y, int z);
    void createDoorInfo(int x, int y, int z);
    bool hasQuery(int x, int y, int z);
    bool isDoor(int x, int y, int z);

public:
    void load(CompoundTag* tag);
    void save(CompoundTag* tag);
};
