#pragma once

#include <vector>

#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class SilverfishModel : public Model {
private:
    static const int BODY_COUNT = 7;

private:
    std::vector<ModelPart*> bodyParts;
    std::vector<ModelPart*> bodyLayers;
    float zPlacement[BODY_COUNT];

    static const int BODY_SIZES[BODY_COUNT][3];

    static const int BODY_TEXS[BODY_COUNT][2];

public:
    SilverfishModel();

    int modelVersion();
    void render(std::shared_ptr<Entity> entity, float time, float r, float bob,
                float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim = 0);
};