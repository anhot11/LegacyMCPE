#pragma once

#include <cstdint>

class Random {
private:
    int64_t seed;
    bool haveNextNextGaussian;
    double nextNextGaussian;

protected:
    int next(int bits);

public:
    Random();
    Random(int64_t seed);
    void setSeed(int64_t s);
    void nextBytes(uint8_t* bytes, unsigned int count);
    double nextDouble();
    double nextGaussian();
    int nextInt();
    int nextInt(int to);
    int nextInt(int minInclusive, int maxInclusive);
    float nextFloat();
    float nextFloat(float min, float max);
    double nextDouble(double min, double max);
    int64_t nextLong();
    bool nextBoolean();
};