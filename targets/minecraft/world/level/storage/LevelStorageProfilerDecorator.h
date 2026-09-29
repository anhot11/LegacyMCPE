#pragma once

#include <string>

#include "LevelStorage.h"

class ConsoleSaveFile;

class LevelStorageProfilerDecorator : public LevelStorage {
private:
    /* final */ LevelStorage* capsulated;

public:
    virtual ConsoleSaveFile* getSaveFile() { return capsulated->getSaveFile(); }

public:
    LevelStorageProfilerDecorator(LevelStorage* capsulated);
    LevelData* prepareLevel();
    void checkSession();
    ChunkStorage* createChunkStorage(Dimension* dimension);
    void saveLevelData(LevelData* levelData,
                       std::vector<std::shared_ptr<Player> >* players);
    void saveLevelData(LevelData* levelData);
    PlayerIO* getPlayerIO();
    void closeAll();
    ConsoleSavePath getDataFile(const std::string& id);
    std::string getLevelId();
};