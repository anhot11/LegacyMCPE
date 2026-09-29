#pragma once
#include "minecraft/client/model/geom/Model.h"
#include "minecraft/client/model/geom/ModelPart.h"

class ModelPart;

class BoatModel : public Model {
public:
    ModelPart* cubes[5];
    BoatModel();
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
};