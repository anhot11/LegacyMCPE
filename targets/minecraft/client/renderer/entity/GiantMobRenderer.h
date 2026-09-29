#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class GiantMobRenderer : public MobRenderer {
private:
    static ResourceLocation ZOMBIE_LOCATION;
    float _scale;

public:
    GiantMobRenderer(Model* model, float shadow, float scale);

protected:
    virtual void scale(std::shared_ptr<LivingEntity> mob, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};