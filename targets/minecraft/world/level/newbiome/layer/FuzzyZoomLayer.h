#pragma once

#include <stdint.h>

#include <memory>

#include "Layer.h"

class FuzzyZoomLayer : public Layer {
public:
    FuzzyZoomLayer(int64_t seedMixup, std::shared_ptr<Layer> parent);
    std::vector<int> getArea(int xo, int yo, int w, int h);

protected:
    int random(int a, int b);
    int random(int a, int b, int c, int d);

public:
    static std::shared_ptr<Layer> zoom(int64_t seed, std::shared_ptr<Layer> sup,
                                       int count);
};