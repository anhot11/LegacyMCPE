#pragma once

#include "Layer.h"

class FlatLayer : public Layer {
private:
    int val;

public:
    FlatLayer(int val);
    std::vector<int> getArea(int xo, int yo, int w, int h);
};