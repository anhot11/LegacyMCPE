#pragma once
#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class SlimeModel : public Model {
public:
    ModelPart* cube;
    ModelPart *eye0, *eye1, *mouth;

    SlimeModel(int vOffs);

    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
};