#pragma once

#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class SkeletonHeadModel : public Model {
public:
    ModelPart* head;

private:
    void _init(int u, int v, int tw, int th);

public:
    SkeletonHeadModel();
    SkeletonHeadModel(int u, int v, int tw, int th);

    void render(std::shared_ptr<Entity> entity, float time, float r, float bob,
                float yRot, float xRot, float scale, bool usecompiled);
    void setupAnim(float time, float r, float bob, float yRot, float xRot,
                   float scale, std::shared_ptr<Entity> entity,
                   unsigned int uiBitmaskOverrideAnim = 0);
};