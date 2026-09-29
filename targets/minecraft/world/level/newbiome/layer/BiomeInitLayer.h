#pragma once

#include <stdint.h>

#include <memory>
#include <vector>

#include "Layer.h"

class LevelType;
class Biome;

class BiomeInitLayer : public Layer {
private:
    std::vector<Biome*> startBiomes;

public:
    BiomeInitLayer(int64_t seed, std::shared_ptr<Layer> parent,
                   LevelType* levelType);
    virtual ~BiomeInitLayer();
    std::vector<int> getArea(int xo, int yo, int w, int h);
};