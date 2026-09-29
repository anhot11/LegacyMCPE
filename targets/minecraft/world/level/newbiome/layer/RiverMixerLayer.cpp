
#include "minecraft/world/level/newbiome/layer/RiverMixerLayer.h"

#include <stdint.h>

#include <memory>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

RiverMixerLayer::RiverMixerLayer(int64_t seed, std::shared_ptr<Layer> biomes,
                                 std::shared_ptr<Layer> rivers)
    : Layer(seed) {
    this->biomes = biomes;
    this->rivers = rivers;
}

void RiverMixerLayer::init(int64_t seed) {
    biomes->init(seed);
    rivers->init(seed);
    Layer::init(seed);
}

std::vector<int> RiverMixerLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> b = biomes->getArea(xo, yo, w, h);
    std::vector<int> r = rivers->getArea(xo, yo, w, h);

    std::vector<int> result(w * h);
    for (int i = 0; i < w * h; i++) {
        if (b[i] == Biome::ocean->id) {
            result[i] = b[i];

        } else {
            if (r[i] >= 0) {
                if (b[i] == Biome::iceFlats->id)
                    result[i] = Biome::frozenRiver->id;
                else if (b[i] == Biome::mushroomIsland->id ||
                         b[i] == Biome::mushroomIslandShore->id)
                    result[i] =
                        Biome::mushroomIsland
                            ->id;  // 4J - don't make mushroom island shores as
                                   // we don't have any island left once we do
                                   // this as our islands are small (this used
                                   // to change to mushroomIslandShore)
                else
                    result[i] = r[i];
            } else {
                result[i] = b[i];
            }
        }
    }

    return result;
}
