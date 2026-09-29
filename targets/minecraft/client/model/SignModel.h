#pragma once

#include "minecraft/client/model/geom/Model.h"

class Cube;
class ModelPart;

class SignModel : public Model {
public:
    using Model::render;
    ModelPart* cube;
    ModelPart* cube2;

    SignModel();
    void render(bool usecompiled);
};
