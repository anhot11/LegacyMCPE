#include "DemoLevel.h"

#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LevelSettings.h"
#include "minecraft/world/level/LevelType.h"
#include "minecraft/world/level/chunk/ChunkSource.h"
#include "minecraft/world/level/storage/LevelData.h"

class LevelStorage;

LevelSettings DemoLevel::DEMO_LEVEL_SETTINGS =
    LevelSettings(DemoLevel::DEMO_LEVEL_SEED, GameType::SURVIVAL, false, false,
                  false, LevelType::lvl_normal_1_1, LEVEL_MAX_WIDTH, 1.0);

DemoLevel::DemoLevel(std::shared_ptr<LevelStorage> levelStorage,
                     const std::string& levelName)
    : Level(levelStorage, levelName, &DEMO_LEVEL_SETTINGS) {}

void DemoLevel::setInitialSpawn() {
    levelData->setSpawn(DEMO_SPAWN_X, DEMO_SPAWN_Y, DEMO_SPAWN_Z);
}