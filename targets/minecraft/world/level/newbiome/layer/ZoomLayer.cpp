
#include "minecraft/world/level/newbiome/layer/ZoomLayer.h"

#include <stdint.h>

#include <algorithm>
#include <memory>
#include <vector>

#include "minecraft/world/level/newbiome/layer/Layer.h"

ZoomLayer::ZoomLayer(int64_t seedMixup, std::shared_ptr<Layer> parent)
    : Layer(seedMixup) {
    this->parent = parent;
}

std::vector<int> ZoomLayer::getArea(int xo, int yo, int w, int h) {
    int px = xo >> 1;
    int py = yo >> 1;
    int pw = (w >> 1) + 3;
    int ph = (h >> 1) + 3;
    std::vector<int> p = parent->getArea(px, py, pw, ph);

    std::vector<int> tmp(pw * ph * 4);
    // 4jcraft added casts to unsigned
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
            tmp.begin() + (y + (yo & 1)) * (unsigned int)(pw << 1) + (xo & 1),
            tmp.begin() + (y + (yo & 1)) * (unsigned int)(pw << 1) + (xo & 1) +
                w,
            result.begin() + y * w);
    }
    return result;
}

int ZoomLayer::random(int a, int b) { return nextRandom(2) == 0 ? a : b; }

int ZoomLayer::random(int a, int b, int c, int d) {
    if (b == c && c == d) return b;
    if (a == b && a == c) return a;
    if (a == b && a == d) return a;
    if (a == c && a == d) return a;

    if (a == b && c != d) return a;
    if (a == c && b != d) return a;
    if (a == d && b != c) return a;

    if (b == a && c != d) return b;
    if (b == c && a != d) return b;
    if (b == d && a != c) return b;

    if (c == a && b != d) return c;
    if (c == b && a != d) return c;
    if (c == d && a != b) return c;

    if (d == a && b != c) return c;
    if (d == b && a != c) return c;
    if (d == c && a != b) return c;

    int s = nextRandom(4);
    if (s == 0) return a;
    if (s == 1) return b;
    if (s == 2) return c;
    return d;
}

std::shared_ptr<Layer> ZoomLayer::zoom(int64_t seed, std::shared_ptr<Layer> sup,
                                       int count) {
    std::shared_ptr<Layer> result = sup;
    for (int i = 0; i < count; i++) {
        result = std::make_shared<ZoomLayer>(seed + i, result);
    }
    return result;
}
