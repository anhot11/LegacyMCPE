#pragma once

#include <stdint.h>

#include <memory>

#include "Layer.h"

class SwampRiversLayer : public Layer {
public:
    SwampRiversLayer(int64_t seed, std::shared_ptr<Layer> parent);

    std::vector<int> getArea(int xo, int yo, int w, int h);
};