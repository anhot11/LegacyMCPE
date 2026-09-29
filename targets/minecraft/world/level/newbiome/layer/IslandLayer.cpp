
#include "minecraft/world/level/newbiome/layer/IslandLayer.h"

#include <stdint.h>

#include <vector>

#include "minecraft/world/level/newbiome/layer/Layer.h"

IslandLayer::IslandLayer(int64_t seedMixup) : Layer(seedMixup) {}

std::vector<int> IslandLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            initRandom(xo + x, yo + y);
            result[x + y * w] = (nextRandom(10) == 0) ? 1 : 0;
        }
    }
    // if (0, 0) is located here, place an island
    if (xo > -w && xo <= 0 && yo > -h && yo <= 0) {
        result[-xo + -yo * w] = 1;
    }
    return result;
}
