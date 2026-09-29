#pragma once
#include <format>
#include <vector>

class Synth {
public:
    virtual double getValue(double x, double y) = 0;
    virtual ~Synth() {}

    std::vector<double> create(int width, int height);
};
