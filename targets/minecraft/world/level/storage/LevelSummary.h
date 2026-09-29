#pragma once

#include <stdint.h>

#include <string>

class GameType;

class LevelSummary {
    const std::string levelId;
    const std::string levelName;
    const int64_t lastPlayed;
    const int64_t sizeOnDisk;
    const bool requiresConversion;
    GameType* gameMode;
    const bool hardcore;
    const bool _hasCheats;

public:
    LevelSummary(const std::string& levelId, const std::string& levelName,
                 int64_t lastPlayed, int64_t sizeOnDisk, GameType* gameMode,
                 bool requiresConversion, bool hardcore, bool hasCheats);
    std::string getLevelId();
    std::string getLevelName();
    int64_t getSizeOnDisk();
    bool isRequiresConversion();
    int64_t getLastPlayed();
    int compareTo(LevelSummary* rhs);
    GameType* getGameMode();
    bool isHardcore();
    bool hasCheats();
};
