#include "PerlinNoise.h"

#include <stdint.h>

#include "java/Random.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/level/levelgen/synth/ImprovedNoise.h"

PerlinNoise::PerlinNoise(int levels) {
    Random random;
    init(&random, levels);
}

PerlinNoise::PerlinNoise(Random* random, int levels) { init(random, levels); }

void PerlinNoise::init(Random* random, int levels) {
    this->levels = levels;
    noiseLevels = new ImprovedNoise*[levels];
    for (int i = 0; i < levels; i++) {
        noiseLevels[i] = new ImprovedNoise(random);
    }
}

PerlinNoise::~PerlinNoise() {
    for (int i = 0; i < levels; i++) {
        delete noiseLevels[i];
    }
    delete[] noiseLevels;
}

double PerlinNoise::getValue(double x, double y) {
    double value = 0;
    double pow = 1;

    for (int i = 0; i < levels; i++) {
        value += noiseLevels[i]->getValue(x * pow, y * pow) / pow;
        pow /= 2;
    }

    return value;
}

double PerlinNoise::getValue(double x, double y, double z) {
    double value = 0;
    double pow = 1;

    for (int i = 0; i < levels; i++) {
        value += noiseLevels[i]->getValue(x * pow, y * pow, z * pow) / pow;
        pow /= 2;
    }

    return value;
}

std::vector<double> PerlinNoise::getRegion(std::vector<double>& buffer, int x,
                                           int y, int z, int xSize, int ySize,
                                           int zSize, double xScale,
                                           double yScale, double zScale) {
    if (buffer.empty())
        buffer = std::vector<double>(xSize * ySize * zSize);
    else
        for (unsigned int i = 0; i < buffer.size(); i++) buffer[i] = 0;

    double pow = 1;

    for (int i = 0; i < levels; i++) {
        //            value += noiseLevels[i].getValue(x * pow, y * pow, z *
        //            pow) / pow;
        double xx = x * pow * xScale;
        double yy = y * pow * yScale;
        double zz = z * pow * zScale;
        int64_t xb = Mth::lfloor(xx);
        int64_t zb = Mth::lfloor(zz);
        xx -= xb;
        zz -= zb;
        xb %= 16777216;
        zb %= 16777216;
        xx += xb;
        zz += zb;
        noiseLevels[i]->add(buffer, xx, yy, zz, xSize, ySize, zSize,
                            xScale * pow, yScale * pow, zScale * pow, pow);
        pow /= 2;
    }

    return buffer;
}

std::vector<double> PerlinNoise::getRegion(std::vector<double>& sr, int x,
                                           int z, int xSize, int zSize,
                                           double xScale, double zScale,
                                           double pow) {
    return getRegion(sr, x, 10, z, xSize, 1, zSize, xScale, 1, zScale);
}