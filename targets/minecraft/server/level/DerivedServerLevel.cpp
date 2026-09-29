#include "DerivedServerLevel.h"

#include "minecraft/server/level/ServerLevel.h"
#include "minecraft/world/level/storage/DerivedLevelData.h"
#include "minecraft/world/level/storage/SavedDataStorage.h"

class LevelSettings;
class LevelStorage;
class MinecraftServer;

DerivedServerLevel::DerivedServerLevel(
    MinecraftServer* server, std::shared_ptr<LevelStorage> levelStorage,
    const std::string& levelName, int dimension, LevelSettings* levelSettings,
    ServerLevel* wrapped)
    : ServerLevel(server, levelStorage, levelName, dimension, levelSettings) {
    // 4J-PB - we're going to override the savedDataStorage, so we need to
    // delete the current one
    if (this->savedDataStorage) {
        delete this->savedDataStorage;
        this->savedDataStorage = nullptr;
    }
    this->savedDataStorage = wrapped->savedDataStorage;
    levelData = new DerivedLevelData(wrapped->getLevelData());
}

DerivedServerLevel::~DerivedServerLevel() {
    // we didn't allocate savedDataStorage here, so we don't want the level
    // destructor to delete it
    this->savedDataStorage = nullptr;
}

void DerivedServerLevel::saveLevelData() {
    // Do nothing?
    // Do nothing!
}