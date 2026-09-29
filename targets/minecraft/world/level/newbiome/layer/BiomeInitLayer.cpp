#include "BiomeInitLayer.h"

#include "minecraft/world/level/LevelType.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

BiomeInitLayer::~BiomeInitLayer() {}

BiomeInitLayer::BiomeInitLayer(int64_t seed, std::shared_ptr<Layer> parent,
                               LevelType* levelType)
    : Layer(seed) {
    this->parent = parent;

    if (levelType == LevelType::lvl_normal_1_1) {
        startBiomes = std::vector<Biome*>(6);
        startBiomes[0] = Biome::desert;
        startBiomes[1] = Biome::forest;
        startBiomes[2] = Biome::extremeHills;
        startBiomes[3] = Biome::swampland;
        startBiomes[4] = Biome::plains;
        startBiomes[5] = Biome::taiga;
    } else {
        startBiomes = std::vector<Biome*>(7);
        startBiomes[0] = Biome::desert;
        startBiomes[1] = Biome::forest;
        startBiomes[2] = Biome::extremeHills;
        startBiomes[3] = Biome::swampland;
        startBiomes[4] = Biome::plains;
        startBiomes[5] = Biome::taiga;
        startBiomes[6] = Biome::jungle;
    }
}

std::vector<int> BiomeInitLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> b = parent->getArea(xo, yo, w, h);

    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            initRandom(x + xo, y + yo);
            int old = b[x + y * w];
            if (old == 0) {
                result[x + y * w] = 0;
            } else if (old == Biome::mushroomIsland->id) {
                result[x + y * w] = old;
            } else if (old == 1) {
                result[x + y * w] =
                    startBiomes[nextRandom(startBiomes.size())]->id;
            } else {
                int isTaiga = startBiomes[nextRandom(startBiomes.size())]->id;
                if (isTaiga == Biome::taiga->id) {
                    result[x + y * w] = isTaiga;
                } else {
                    result[x + y * w] = Biome::iceFlats->id;
                }
            }
        }
    }

    return result;
}
