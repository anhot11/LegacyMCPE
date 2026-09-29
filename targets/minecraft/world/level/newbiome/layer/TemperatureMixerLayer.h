#pragma once

#include <memory>

#include "Layer.h"

class TemperatureMixerLayer : public Layer {
private:
    std::shared_ptr<Layer> temp;
    int layer;

public:
    TemperatureMixerLayer(std::shared_ptr<Layer> temp,
                          std::shared_ptr<Layer> parent, int layer);

    virtual std::vector<int> getArea(int xo, int yo, int w, int h);
};