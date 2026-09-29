#pragma once
#include <memory>
#include <string>

#include "ServerLevel.h"

class LevelSettings;
class LevelStorage;
class MinecraftServer;

class DerivedServerLevel : public ServerLevel {
public:
    DerivedServerLevel(MinecraftServer* server,
                       std::shared_ptr<LevelStorage> levelStorage,
                       const std::string& levelName, int dimension,
                       LevelSettings* levelSettings, ServerLevel* wrapped);
    ~DerivedServerLevel();

protected:
    void saveLevelData();
};