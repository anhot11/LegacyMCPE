#pragma once
#include <vector>

#include "minecraft/client/model/geom/Model.h"
#include "minecraft/client/model/geom/ModelPart.h"

class ModelPart;

class WitherBossModel : public Model {
private:
    std::vector<ModelPart*> upperBodyParts;
    std::vector<ModelPart*> heads;

public:
    WitherBossModel();

    int modelVersion();
    virtual void render(std::shared_ptr<Entity> entity, float time, float r,
                        float bob, float yRot, float xRot, float scale,
                        bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot,
                           float xRot, float scale,
                           std::shared_ptr<Entity> entity,
                           unsigned int uiBitmaskOverrideAnim = 0);
    virtual void prepareMobModel(std::shared_ptr<LivingEntity> mob, float time,
                                 float r, float a);
};