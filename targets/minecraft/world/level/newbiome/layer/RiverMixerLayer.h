#pragma once

#include <stdint.h>

#include <memory>

#include "Layer.h"

class RiverMixerLayer : public Layer {
private:
    std::shared_ptr<Layer> biomes;
    std::shared_ptr<Layer> rivers;

public:
    RiverMixerLayer(int64_t seed, std::shared_ptr<Layer> biomes,
                    std::shared_ptr<Layer> rivers);

    virtual void init(int64_t seed);
    virtual std::vector<int> getArea(int xo, int yo, int w, int h);
};