
#include "minecraft/world/level/newbiome/layer/AddSnowLayer.h"

#include <stdint.h>

#include <memory>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

AddSnowLayer::AddSnowLayer(int64_t seedMixup, std::shared_ptr<Layer> parent)
    : Layer(seedMixup) {
    this->parent = parent;
}

std::vector<int> AddSnowLayer::getArea(int xo, int yo, int w, int h) {
    int px = xo - 1;
    int py = yo - 1;
    int pw = w + 2;
    int ph = h + 2;
    std::vector<int> p = parent->getArea(px, py, pw, ph);

    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int c = p[(x + 1) + (y + 1) * pw];
            initRandom(x + xo, y + yo);
            if (c == 0) {
                result[x + y * w] = 0;
            } else {
                int r = nextRandom(5);
                if (r == 0)
                    r = Biome::iceFlats->id;
                else
                    r = 1;
                result[x + y * w] = r;
            }
        }
    }
    return result;
}
