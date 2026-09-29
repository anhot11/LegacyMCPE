#pragma once
#include <format>
#include <vector>

#include "ImprovedNoise.h"
#include "Synth.h"

class ImprovedNoise;
class Random;

class PerlinNoise : public Synth {
    friend class PerlinNoise_SPU;

private:
    ImprovedNoise** noiseLevels;
    int levels;

public:
    PerlinNoise(int levels);
    PerlinNoise(Random* random, int levels);

    void init(Random* random, int levels);
    ~PerlinNoise();

    virtual double getValue(double x, double y);
    double getValue(double x, double y, double z);
    std::vector<double> getRegion(std::vector<double>& buffer, int x, int y,
                                  int z, int xSize, int ySize, int zSize,
                                  double xScale, double yScale, double zScale);
    std::vector<double> getRegion(std::vector<double>& sr, int x, int z,
                                  int xSize, int zSize, double xScale,
                                  double zScale, double pow);
};
