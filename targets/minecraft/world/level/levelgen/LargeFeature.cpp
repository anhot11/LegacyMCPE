
#include "LargeFeature.h"

#include <stdint.h>

#include <format>
#include <string>

#include "java/Random.h"
#include "minecraft/world/level/Level.h"

class ChunkSource;

const std::string LargeFeature::STRONGHOLD = "StrongHold";

LargeFeature::LargeFeature() {
    radius = 8;
    random = new Random();
}

LargeFeature::~LargeFeature() { delete random; }

void LargeFeature::apply(ChunkSource* ChunkSource, Level* level, int xOffs,
                         int zOffs, std::vector<uint8_t>& blocks) {
    int r = radius;
    this->level = level;

    random->setSeed(level->getSeed());
    int64_t xScale = random->nextLong();
    int64_t zScale = random->nextLong();

    for (int x = xOffs - r; x <= xOffs + r; x++) {
        for (int z = zOffs - r; z <= zOffs + r; z++) {
            int64_t xx = (uint64_t)x * xScale;
            int64_t zz = (uint64_t)z * zScale;
            random->setSeed(xx ^ zz ^ level->getSeed());
            addFeature(level, x, z, xOffs, zOffs, blocks);
        }
    }
}