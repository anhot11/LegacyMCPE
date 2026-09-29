
#include "minecraft/world/level/newbiome/layer/AddMushroomIslandLayer.h"

#include <stdint.h>

#include <memory>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

AddMushroomIslandLayer::AddMushroomIslandLayer(int64_t seedMixup,
                                               std::shared_ptr<Layer> parent)
    : Layer(seedMixup) {
    this->parent = parent;
}

std::vector<int> AddMushroomIslandLayer::getArea(int xo, int yo, int w, int h) {
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
            if (c == 0 && (n1 == 0 && n2 == 0 && n3 == 0 && n4 == 0) &&
                nextRandom(100) == 0) {
                result[x + y * w] = Biome::mushroomIsland->id;
            } else {
                result[x + y * w] = c;
            }
        }
    }
    return result;
}
