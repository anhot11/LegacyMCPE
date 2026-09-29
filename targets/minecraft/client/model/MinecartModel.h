#pragma once
#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class MinecartModel : public Model {
public:
    static const int MINECART_LENGTH = 6;

    ModelPart* cubes[MINECART_LENGTH];

    MinecartModel();
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
};
