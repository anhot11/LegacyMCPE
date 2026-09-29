#pragma once

#include <memory>

#include "Layer.h"

class TemperatureLayer : public Layer {
public:
    TemperatureLayer(std::shared_ptr<Layer> parent);

    virtual std::vector<int> getArea(int xo, int yo, int w, int h);
};