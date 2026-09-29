#pragma once
#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class GhastModel : public Model {
public:
    static const int TENTACLESLENGTH = 9;
    ModelPart* body;
    ModelPart* tentacles[TENTACLESLENGTH];

    GhastModel();
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim = 0);
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
};