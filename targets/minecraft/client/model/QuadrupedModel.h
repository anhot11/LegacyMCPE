#pragma once
#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class QuadrupedModel : public Model {
public:
    ModelPart *head, *body, *leg0, *leg1, *leg2, *leg3;

    QuadrupedModel(int legSize, float g);
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim = 0);
    void render(QuadrupedModel* model, float scale, bool usecompiled);
};