#pragma once

#include "nbt/CompoundTag.h"

// If we have more than MAX_PLAYER_DATA_SAVES player.dat's then we delete the
// oldest ones This value can be no higher than MAXIMUM_MAP_SAVE_DATA/3 (3 being
// the number of dimensions in future versions)
#define MAX_PLAYER_DATA_SAVES 80

class Player;

class PlayerIO {
public:
    virtual void save(std::shared_ptr<Player> player) = 0;
    virtual CompoundTag* load(std::shared_ptr<Player> player) = 0;
    virtual CompoundTag* loadPlayerDataTag(
        PlayerUID xuid) = 0;  // 4J Changed from std::string name to xuid

    // 4J Added
    virtual void clearOldPlayerFiles() = 0;
    virtual void saveMapIdLookup() = 0;
    virtual void deleteMapFilesForPlayer(std::shared_ptr<Player> player) = 0;
    virtual void saveAllCachedData() = 0;
};