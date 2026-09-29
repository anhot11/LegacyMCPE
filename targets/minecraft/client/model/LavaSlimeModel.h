#pragma once
#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class LavaSlimeModel : public Model {
    static const int BODYCUBESLENGTH = 8;
    ModelPart* bodyCubes[BODYCUBESLENGTH];
    ModelPart* insideCube;

public:
    LavaSlimeModel();
    int getModelVersion();
    virtual void prepareMobModel(std::shared_ptr<LivingEntity> mob, float time,
                                 float r, float a);
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
};
