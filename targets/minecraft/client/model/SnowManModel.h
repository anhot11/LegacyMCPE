#pragma once
#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class SnowManModel : public Model {
public:
    ModelPart *piece1, *piece2, *head;
    ModelPart *arm1, *arm2;

    SnowManModel();
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim = 0);
    void render(std::shared_ptr<Entity> entity, float time, float r, float bob,
                float yRot, float xRot, float scale, bool usecompiled);
};
