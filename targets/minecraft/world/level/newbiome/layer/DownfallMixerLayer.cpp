
#include "minecraft/world/level/newbiome/layer/DownfallMixerLayer.h"

#include <memory>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

DownfallMixerLayer::DownfallMixerLayer(std::shared_ptr<Layer> downfall,
                                       std::shared_ptr<Layer> parent, int layer)
    : Layer(0) {
    this->parent = parent;
    this->downfall = downfall;
    this->layer = layer;
}

std::vector<int> DownfallMixerLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> b = parent->getArea(xo, yo, w, h);
    std::vector<int> d = downfall->getArea(xo, yo, w, h);

    std::vector<int> result(w * h);
    for (int i = 0; i < w * h; i++) {
        result[i] =
            d[i] + (Biome::biomes[b[i]]->getDownfallInt() - d[i]) / (layer + 1);
    }

    return result;
}
