#pragma once
#include <vector>

#include "minecraft/client/model/geom/Model.h"

class ModelPart;

class BlazeModel : public Model {
private:
    std::vector<ModelPart*> upperBodyParts;
    ModelPart* head;

public:
    BlazeModel();
    int modelVersion();
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim = 0);
};
