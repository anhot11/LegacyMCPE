
#include "minecraft/world/level/newbiome/layer/SmoothLayer.h"

#include <stdint.h>

#include <memory>
#include <vector>

#include "minecraft/world/level/newbiome/layer/Layer.h"

SmoothLayer::SmoothLayer(int64_t seedMixup, std::shared_ptr<Layer> parent)
    : Layer(seedMixup) {
    this->parent = parent;
}

std::vector<int> SmoothLayer::getArea(int xo, int yo, int w, int h) {
    int px = xo - 1;
    int py = yo - 1;
    int pw = w + 2;
    int ph = h + 2;
    std::vector<int> p = parent->getArea(px, py, pw, ph);

    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int l = p[(x + 0) + (y + 1) * pw];
            int r = p[(x + 2) + (y + 1) * pw];
            int u = p[(x + 1) + (y + 0) * pw];
            int d = p[(x + 1) + (y + 2) * pw];
            int c = p[(x + 1) + (y + 1) * pw];
            if (l == r && u == d) {
                initRandom((x + xo), (y + yo));
                if (nextRandom(2) == 0)
                    c = l;
                else
                    c = u;

            } else {
                if (l == r) c = l;
                if (u == d) c = u;
            }
            result[x + y * w] = c;
        }
    }

    return result;
}
