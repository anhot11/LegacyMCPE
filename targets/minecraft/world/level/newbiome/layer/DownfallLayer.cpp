
#include "minecraft/world/level/newbiome/layer/DownfallLayer.h"

#include <memory>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

DownfallLayer::DownfallLayer(std::shared_ptr<Layer> parent) : Layer(0) {
    this->parent = parent;
}

std::vector<int> DownfallLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> b = parent->getArea(xo, yo, w, h);

    std::vector<int> result(w * h);
    for (int i = 0; i < w * h; i++) {
        result[i] = Biome::biomes[b[i]]->getDownfallInt();
    }

    return result;
}
