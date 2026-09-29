#pragma once

#include <stdint.h>

#include <memory>

#include "Layer.h"

class VoronoiZoom : public Layer {
public:
    VoronoiZoom(int64_t seedMixup, std::shared_ptr<Layer> parent);

    virtual std::vector<int> getArea(int xo, int yo, int w, int h);

protected:
    int random(int a, int b);
    int random(int a, int b, int c, int d);
};