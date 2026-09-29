#include "JungleBiome.h"

#include <vector>

#include "java/Class.h"
#include "java/Random.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/biome/BiomeDecorator.h"
#include "minecraft/world/level/levelgen/feature/BasicTreeFeature.h"
#include "minecraft/world/level/levelgen/feature/GroundBushFeature.h"
#include "minecraft/world/level/levelgen/feature/MegaTreeFeature.h"
#include "minecraft/world/level/levelgen/feature/TallGrassFeature.h"
#include "minecraft/world/level/levelgen/feature/TreeFeature.h"
#include "minecraft/world/level/levelgen/feature/VinesFeature.h"
#include "minecraft/world/level/tile/LeafTile.h"
#include "minecraft/world/level/tile/TallGrassPlantTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/TreeTile.h"

JungleBiome::JungleBiome(int id) : Biome(id) {
    decorator->treeCount = 50;
    decorator->grassCount = 25;
    decorator->flowerCount = 4;

    enemies.push_back(new MobSpawnerData(eTYPE_OCELOT, 2, 1, 1));

    // make chicken a lot more common in the jungle
    friendlies.push_back(new MobSpawnerData(eTYPE_CHICKEN, 10, 4, 4));
}

Feature* JungleBiome::getTreeFeature(Random* random) {
    if (random->nextInt(10) == 0) {
        return new BasicTree(
            false);  // 4J used to return member fancyTree, now returning newly
                     // created object so that caller can be consistently
                     // resposible for cleanup
    }
    if (random->nextInt(2) == 0) {
        return new GroundBushFeature(TreeTile::JUNGLE_TRUNK,
                                     LeafTile::NORMAL_LEAF);
    }
    if (random->nextInt(3) == 0) {
        return new MegaTreeFeature(false, 10 + random->nextInt(20),
                                   TreeTile::JUNGLE_TRUNK,
                                   LeafTile::JUNGLE_LEAF);
    }
    return new TreeFeature(false, 4 + random->nextInt(7),
                           TreeTile::JUNGLE_TRUNK, LeafTile::JUNGLE_LEAF, true);
}

Feature* JungleBiome::getGrassFeature(Random* random) {
    if (random->nextInt(4) == 0) {
        return new TallGrassFeature(Tile::tallgrass_Id, TallGrass::FERN);
    }
    return new TallGrassFeature(Tile::tallgrass_Id, TallGrass::TALL_GRASS);
}

void JungleBiome::decorate(Level* level, Random* random, int xo, int zo) {
    Biome::decorate(level, random, xo, zo);

    VinesFeature* vines = new VinesFeature();

    for (int i = 0; i < 50; i++) {
        int x = xo + random->nextInt(16) + 8;
        int y = Level::genDepth / 2;
        int z = zo + random->nextInt(16) + 8;
        vines->place(level, random, x, y, z);
    }
}