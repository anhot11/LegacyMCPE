#include "TheEndBiome.h"

#include <stdint.h>

#include <vector>

#include "TheEndBiomeDecorator.h"
#include "java/Class.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/tile/Tile.h"

TheEndBiome::TheEndBiome(int id) : Biome(id) {
    enemies.clear();
    friendlies.clear();
    friendlies_chicken.clear();  // 4J added
    friendlies_wolf.clear();     // 4J added
    waterFriendlies.clear();
    ambientFriendlies.clear();

    enemies.push_back(new MobSpawnerData(eTYPE_ENDERMAN, 10, 4, 4));
    topMaterial = (uint8_t)Tile::dirt_Id;
    material = (uint8_t)Tile::dirt_Id;

    decorator = new TheEndBiomeDecorator(this);
}

// 4J Stu - Don't need override
// int TheEndBiome::getSkyColor(float temp)
//{
//	return 0x000000;
//}