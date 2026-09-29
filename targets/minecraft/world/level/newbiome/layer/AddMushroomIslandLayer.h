#pragma once
#include <stdint.h>

#include <memory>

#include "Layer.h"

class AddMushroomIslandLayer : public Layer {
public:
    AddMushroomIslandLayer(int64_t seedMixup, std::shared_ptr<Layer> parent);
    virtual std::vector<int> getArea(int xo, int yo, int w, int h);
};