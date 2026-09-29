#pragma once

#include <stdint.h>

#include <format>
#include <memory>
#include <vector>

class LevelType;

#ifndef _CONTENT_PACKAGE
#define _BIOME_OVERRIDE
#endif

class Layer {
private:
    int64_t seed;

protected:
    std::shared_ptr<Layer> parent;

private:
    int64_t rval;
    int64_t seedMixup;

public:
    static std::vector<std::shared_ptr<Layer>> getDefaultLayers(
        int64_t seed, LevelType* levelType);

    Layer(int64_t seedMixup);

    virtual void init(int64_t seed);
    virtual void initRandom(int64_t x, int64_t y);

protected:
    int nextRandom(int max);

public:
    virtual std::vector<int> getArea(int xo, int yo, int w, int h) = 0;
};