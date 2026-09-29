#pragma once
#include <stdint.h>

#include <memory>

#include "Layer.h"

class GrowMushroomIslandLayer : public Layer {
public:
    GrowMushroomIslandLayer(int64_t seedMixup, std::shared_ptr<Layer> parent);
    virtual std::vector<int> getArea(int xo, int yo, int w, int h);
};