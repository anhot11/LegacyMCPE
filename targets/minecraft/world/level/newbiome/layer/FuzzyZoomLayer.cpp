
#include "minecraft/world/level/newbiome/layer/FuzzyZoomLayer.h"

#include <stdint.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "minecraft/world/level/newbiome/layer/Layer.h"

FuzzyZoomLayer::FuzzyZoomLayer(int64_t seedMixup, std::shared_ptr<Layer> parent)
    : Layer(seedMixup) {
    this->parent = parent;
}

std::vector<int> FuzzyZoomLayer::getArea(int xo, int yo, int w, int h) {
    int px = xo >> 1;
    int py = yo >> 1;
    int pw = (w >> 1) + 3;
    int ph = (h >> 1) + 3;
    std::vector<int> p = parent->getArea(px, py, pw, ph);

    // 4jcraft added casts to unsigned to prevent shift of neg value
    std::vector<int> tmp(pw * ph * 4);
    int ww = ((unsigned int)pw << 1);
    for (int y = 0; y < ph - 1; y++) {
        int ry = (unsigned int)y << 1;
        int pp = ry * ww;
        int ul = p[(0 + 0) + (y + 0) * pw];
        int dl = p[(0 + 0) + (y + 1) * pw];
        for (int x = 0; x < pw - 1; x++) {
            initRandom((unsigned int)(x + px) << 1, (unsigned int)(y + py)
                                                        << 1);
            int ur = p[(x + 1) + (y + 0) * pw];
            int dr = p[(x + 1) + (y + 1) * pw];

            tmp[pp] = ul;
            tmp[pp++ + ww] = random(ul, dl);
            tmp[pp] = random(ul, ur);
            tmp[pp++ + ww] = random(ul, ur, dl, dr);

            ul = ur;
            dl = dr;
        }
    }
    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        std::copy(
            tmp.begin() + (y + (yo & 1)) * ((unsigned int)pw << 1) + (xo & 1),
            tmp.begin() + (y + (yo & 1)) * ((unsigned int)pw << 1) + (xo & 1) +
                w,
            result.begin() + y * w);
    }

    return result;
}

int FuzzyZoomLayer::random(int a, int b) { return nextRandom(2) == 0 ? a : b; }

int FuzzyZoomLayer::random(int a, int b, int c, int d) {
    int s = nextRandom(4);
    if (s == 0) return a;
    if (s == 1) return b;
    if (s == 2) return c;
    return d;
}

std::shared_ptr<Layer> FuzzyZoomLayer::zoom(int64_t seed,
                                            std::shared_ptr<Layer> sup,
                                            int count) {
    std::shared_ptr<Layer> result = sup;
    for (int i = 0; i < count; i++) {
        result = std::make_shared<FuzzyZoomLayer>(seed + i, result);
    }
    return result;
}
