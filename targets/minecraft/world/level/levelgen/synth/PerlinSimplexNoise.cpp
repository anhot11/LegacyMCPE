#include "PerlinSimplexNoise.h"

#include "java/Random.h"
#include "minecraft/world/level/levelgen/synth/SimplexNoise.h"

PerlinSimplexNoise::PerlinSimplexNoise(int levels) {
    Random random;
    init(&random, levels);
}

PerlinSimplexNoise::PerlinSimplexNoise(Random* random, int levels) {
    init(random, levels);
    delete random;
}

void PerlinSimplexNoise::init(Random* random, int levels) {
    this->levels = levels;
    noiseLevels = new SimplexNoise*[levels];
    for (int i = 0; i < levels; i++) {
        noiseLevels[i] = new SimplexNoise(random);
    }
}

PerlinSimplexNoise::~PerlinSimplexNoise() {
    for (int i = 0; i < levels; i++) {
        delete noiseLevels[i];
    }
    delete[] noiseLevels;
}

double PerlinSimplexNoise::getValue(double x, double y) {
    double value = 0;
    double pow = 1;

    for (int i = 0; i < levels; i++) {
        value += noiseLevels[i]->getValue(x * pow, y * pow) / pow;
        pow /= 2;
    }

    return value;
}

double PerlinSimplexNoise::getValue(double x, double y, double z) {
    double value = 0;
    double pow = 1;

    for (int i = 0; i < levels; i++) {
        value += noiseLevels[i]->getValue(x * pow, y * pow, z * pow) / pow;
        pow /= 2;
    }

    return value;
}

std::vector<double> PerlinSimplexNoise::getRegion(std::vector<double>& buffer,
                                                  double x, double y, int xSize,
                                                  int ySize, double xScale,
                                                  double yScale,
                                                  double sizeScale) {
    return getRegion(buffer, x, y, xSize, ySize, xScale, yScale, sizeScale,
                     0.5);
}

std::vector<double> PerlinSimplexNoise::getRegion(
    std::vector<double>& buffer, double x, double y, int xSize, int ySize,
    double xScale, double yScale, double sizeScale, double powScale) {
    xScale /= 1.5;
    yScale /= 1.5;

    if (buffer.empty() || (int)buffer.size() < xSize * ySize) {
        buffer = std::vector<double>(xSize * ySize);
    } else
        for (unsigned int i = 0; i < buffer.size(); i++) buffer[i] = 0;

    double pow = 1;
    double scale = 1;
    for (int i = 0; i < levels; i++) {
        noiseLevels[i]->add(buffer, x, y, xSize, ySize, xScale * scale,
                            yScale * scale, 0.55 / pow);
        scale *= sizeScale;
        pow *= powScale;
    }

    return buffer;
}

std::vector<double> PerlinSimplexNoise::getRegion(
    std::vector<double>& buffer, double x, double y, double z, int xSize,
    int ySize, int zSize, double xScale, double yScale, double zScale) {
    xScale /= 1.5;
    yScale /= 1.5;

    if (buffer.empty())
        buffer = std::vector<double>(xSize * ySize * zSize);
    else
        for (unsigned int i = 0; i < buffer.size(); i++) buffer[i] = 0;

    double pow = 1;

    for (int i = 0; i < levels; i++) {
        //            value += noiseLevels[i].getValue(x * pow, y * pow, z *
        //            pow) / pow;
        noiseLevels[i]->add(buffer, x, y, z, xSize, ySize, zSize, xScale * pow,
                            yScale * pow, zScale * pow, 0.55 / pow);
        pow *= 0.5;
    }

    return buffer;
}