#pragma once

#include <string>

#include "LevelStorage.h"
#include "minecraft/world/level/storage/ConsoleSaveFileIO/ConsoleSavePath.h"

class MockedLevelStorage : public LevelStorage {
public:
    virtual LevelData* prepareLevel();
    virtual void checkSession();
    virtual ChunkStorage* createChunkStorage(Dimension* dimension);
    virtual void saveLevelData(LevelData* levelData,
                               std::vector<std::shared_ptr<Player> >* players);
    virtual void saveLevelData(LevelData* levelData);
    virtual PlayerIO* getPlayerIO();
    virtual void closeAll();
    virtual ConsoleSavePath getDataFile(const std::string& id);
    virtual std::string getLevelId();

public:
    virtual ConsoleSaveFile* getSaveFile() { return nullptr; }
};