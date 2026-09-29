
#include "minecraft/world/level/newbiome/layer/TemperatureMixerLayer.h"

#include <memory>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/newbiome/layer/Layer.h"

TemperatureMixerLayer::TemperatureMixerLayer(std::shared_ptr<Layer> temp,
                                             std::shared_ptr<Layer> parent,
                                             int layer)
    : Layer(0) {
    this->parent = parent;
    this->temp = temp;
    this->layer = layer;
}

std::vector<int> TemperatureMixerLayer::getArea(int xo, int yo, int w, int h) {
    std::vector<int> b = parent->getArea(xo, yo, w, h);
    std::vector<int> t = temp->getArea(xo, yo, w, h);

    std::vector<int> result(w * h);
    for (int i = 0; i < w * h; i++) {
        result[i] = t[i] + (Biome::biomes[b[i]]->getTemperatureInt() - t[i]) /
                               (layer * 2 + 1);
    }

    return result;
}
