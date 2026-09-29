
#include "minecraft/world/level/newbiome/layer/SmoothZoomLayer.h"

#include <stdint.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "minecraft/world/level/newbiome/layer/Layer.h"

SmoothZoomLayer::SmoothZoomLayer(int64_t seedMixup,
                                 std::shared_ptr<Layer> parent)
    : Layer(seedMixup) {
    this->parent = parent;
}

std::vector<int> SmoothZoomLayer::getArea(int xo, int yo, int w, int h) {
    int px = xo >> 1;
    int py = yo >> 1;
    int pw = (w >> 1) + 3;
    int ph = (h >> 1) + 3;
    std::vector<int> p = parent->getArea(px, py, pw, ph);

    std::vector<int> tmp(pw * ph * 4);
    int ww = (pw << 1);
    for (int y = 0; y < ph - 1; y++) {
        int ry = y << 1;
        int pp = ry * ww;
        int ul = p[(0 + 0) + (y + 0) * pw];
        int dl = p[(0 + 0) + (y + 1) * pw];
        for (int x = 0; x < pw - 1; x++) {
            initRandom((x + px) << 1, (y + py) << 1);

            int ur = p[(x + 1) + (y + 0) * pw];
            int dr = p[(x + 1) + (y + 1) * pw];

            tmp[pp] = ul;
            tmp[pp++ + ww] = ul + (dl - ul) * (nextRandom(256)) / 256;
            tmp[pp] = ul + (ur - ul) * (nextRandom(256)) / 256;

            int a = ul + (ur - ul) * (nextRandom(256)) / 256;
            int b = dl + (dr - dl) * (nextRandom(256)) / 256;
            tmp[pp++ + ww] = a + (b - a) * (nextRandom(256)) / 256;

            ul = ur;
            dl = dr;
        }
    }
    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        std::copy(tmp.begin() + (y + (yo & 1)) * (pw << 1) + (xo & 1),
                  tmp.begin() + (y + (yo & 1)) * (pw << 1) + (xo & 1) + w,
                  result.begin() + y * w);
    }
    return result;
}

std::shared_ptr<Layer> SmoothZoomLayer::zoom(int64_t seed,
                                             std::shared_ptr<Layer> sup,
                                             int count) {
    std::shared_ptr<Layer> result = sup;
    for (int i = 0; i < count; i++) {
        result = std::make_shared<SmoothZoomLayer>(seed + i, result);
    }
    return result;
}
