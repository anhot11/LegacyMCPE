#pragma once
#include <stdint.h>

#include <memory>

#include "Layer.h"

class ShoreLayer : public Layer {
public:
    ShoreLayer(int64_t seed, std::shared_ptr<Layer> parent);
    virtual std::vector<int> getArea(int xo, int yo, int w, int h);
};