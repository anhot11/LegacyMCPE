#pragma once

#include <memory>

#include "Layer.h"

class DownfallLayer : public Layer {
public:
    DownfallLayer(std::shared_ptr<Layer> parent);
    std::vector<int> getArea(int xo, int yo, int w, int h);
};