#include "SwampRiversLayer.h"

#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

SwampRiversLayer::SwampRiversLayer(int64_t seed, std::shared_ptr<Layer> parent)
    : Layer(seed) {
    this->parent = parent;
}

std::vector<int> SwampRiversLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> b = parent->getArea(xo - 1, yo - 1, w + 2, h + 2);

    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            initRandom(x + xo, y + yo);
            int old = b[(x + 1) + (y + 1) * (w + 2)];
            if ((old == Biome::swampland->id && nextRandom(6) == 0) ||
                ((old == Biome::jungle->id || old == Biome::jungleHills->id) &&
                 nextRandom(8) == 0)) {
                result[x + y * w] = Biome::river->id;
            } else {
                result[x + y * w] = old;
            }
        }
    }

    return result;
}
