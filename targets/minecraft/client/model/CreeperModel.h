#pragma once
#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class CreeperModel : public Model {
public:
    ModelPart *head, *hair, *body, *leg0, *leg1, *leg2, *leg3;

    void _init(float g);  // 4J added
    CreeperModel();
    CreeperModel(float g);
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim = 0);
};