#pragma once

#include <memory>

#include "Layer.h"

class DownfallMixerLayer : public Layer {
private:
    std::shared_ptr<Layer> downfall;
    int layer;

public:
    DownfallMixerLayer(std::shared_ptr<Layer> downfall,
                       std::shared_ptr<Layer> parent, int layer);
    std::vector<int> getArea(int xo, int yo, int w, int h);
};