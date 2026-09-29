#pragma once

#include <stdint.h>

#include <vector>

#include "Layer.h"

class LevelType;

class BiomeOverrideLayer : public Layer {
private:
    static const unsigned int width = 216;
    static const unsigned int height = 216;

    std::vector<uint8_t> m_biomeOverride;

public:
    BiomeOverrideLayer(int seedMixup);
    std::vector<int> getArea(int xo, int yo, int w, int h);
};