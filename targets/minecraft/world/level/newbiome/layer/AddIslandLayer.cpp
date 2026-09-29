
#include "minecraft/world/level/newbiome/layer/AddIslandLayer.h"

#include <stdint.h>

#include <memory>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

AddIslandLayer::AddIslandLayer(int64_t seedMixup, std::shared_ptr<Layer> parent)
    : Layer(seedMixup) {
    this->parent = parent;
}

std::vector<int> AddIslandLayer::getArea(int xo, int yo, int w, int h) {
    int px = xo - 1;
    int py = yo - 1;
    int pw = w + 2;
    int ph = h + 2;
    std::vector<int> p = parent->getArea(px, py, pw, ph);

    std::vector<int> result(w * h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int n1 = p[(x + 0) + (y + 0) * pw];
            int n2 = p[(x + 2) + (y + 0) * pw];
            int n3 = p[(x + 0) + (y + 2) * pw];
            int n4 = p[(x + 2) + (y + 2) * pw];
            int c = p[(x + 1) + (y + 1) * pw];
            initRandom(x + xo, y + yo);
            if (c == 0 && (n1 != 0 || n2 != 0 || n3 != 0 || n4 != 0)) {
                int odds = 1;
                int swap = 1;
                if (n1 != 0 && nextRandom(odds++) == 0) swap = n1;
                if (n2 != 0 && nextRandom(odds++) == 0) swap = n2;
                if (n3 != 0 && nextRandom(odds++) == 0) swap = n3;
                if (n4 != 0 && nextRandom(odds++) == 0) swap = n4;
                if (nextRandom(3) == 0) {
                    result[x + y * w] = swap;
                } else {
                    if (swap == Biome::iceFlats->id)
                        result[x + y * w] = Biome::frozenOcean->id;
                    else
                        result[x + y * w] = 0;
                }
            } else if (c > 0 && (n1 == 0 || n2 == 0 || n3 == 0 || n4 == 0)) {
                if (nextRandom(5) == 0) {
                    if (c == Biome::iceFlats->id)
                        result[x + y * w] = Biome::frozenOcean->id;
                    else
                        result[x + y * w] = 0;
                } else
                    result[x + y * w] = c;
            } else {
                result[x + y * w] = c;
            }
        }
    }

    return result;
}
