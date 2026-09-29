#pragma once

#include "LevelStorageSource.h"

class MemoryLevelStorageSource : public LevelStorageSource {
public:
    MemoryLevelStorageSource();
    std::string getName();
    std::shared_ptr<LevelStorage> selectLevel(const std::string& levelId,
                                              bool createPlayerDir);
    std::vector<LevelSummary*>* getLevelList();
    void clearAll();
    LevelData* getDataTagFor(const std::string& levelId);
    bool isNewLevelIdAcceptable(const std::string& levelId);
    void deleteLevel(const std::string& levelId);
    void renameLevel(const std::string& levelId,
                     const std::string& newLevelName);
    bool isConvertible(const std::string& levelId);
    bool requiresConversion(const std::string& levelId);
    bool convertLevel(const std::string& levelId, ProgressListener* progress);
};