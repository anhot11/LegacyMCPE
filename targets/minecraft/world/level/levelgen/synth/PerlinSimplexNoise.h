#pragma once
#include <format>
#include <vector>

#include "SimplexNoise.h"
#include "Synth.h"

class Random;
class SimplexNoise;

class PerlinSimplexNoise : public Synth {
private:
    SimplexNoise** noiseLevels;
    int levels;

public:
    PerlinSimplexNoise(int levels);
    PerlinSimplexNoise(Random* random, int levels);
    void init(Random* random, int levels);
    ~PerlinSimplexNoise();

    virtual double getValue(double x, double y);
    double getValue(double x, double y, double z);

    std::vector<double> getRegion(std::vector<double>& buffer, double x,
                                  double y, int xSize, int ySize, double xScale,
                                  double yScale, double sizeScale);
    std::vector<double> getRegion(std::vector<double>& buffer, double x,
                                  double y, int xSize, int ySize, double xScale,
                                  double yScale, double sizeScale,
                                  double powScale);
    std::vector<double> getRegion(std::vector<double>& buffer, double x,
                                  double y, double z, int xSize, int ySize,
                                  int zSize, double xScale, double yScale,
                                  double zScale);
};