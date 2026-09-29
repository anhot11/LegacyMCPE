#pragma once

#include <stdint.h>

#include <memory>

#include "Layer.h"

class SmoothZoomLayer : public Layer {
public:
    SmoothZoomLayer(int64_t seedMixup, std::shared_ptr<Layer> parent);

    virtual std::vector<int> getArea(int xo, int yo, int w, int h);
    static std::shared_ptr<Layer> zoom(int64_t seed, std::shared_ptr<Layer> sup,
                                       int count);
};