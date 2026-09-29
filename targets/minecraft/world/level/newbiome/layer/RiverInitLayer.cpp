
#include "minecraft/world/level/newbiome/layer/RiverInitLayer.h"

#include <stdint.h>

#include <memory>
#include <vector>

#include "minecraft/world/level/newbiome/layer/Layer.h"

RiverInitLayer::RiverInitLayer(int64_t seed, std::shared_ptr<Layer> parent)
    : Layer(seed) {
    this->parent = parent;
}

std::vector<int> RiverInitLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> b = parent->getArea(xo, yo, w, h);

    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            initRandom(x + xo, y + yo);
            result[x + y * w] = b[x + y * w] > 0 ? nextRandom(2) + 2 : 0;
        }
    }

    return result;
}
