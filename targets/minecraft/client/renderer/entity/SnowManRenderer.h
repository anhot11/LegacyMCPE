#pragma once
#include "MobRenderer.h"

class SnowManModel;
class ResourceLocation;

class SnowManRenderer : public MobRenderer {
private:
    SnowManModel* model;
    static ResourceLocation SNOWMAN_LOCATION;

public:
    SnowManRenderer();

protected:
    virtual void additionalRendering(std::shared_ptr<LivingEntity> _mob,
                                     float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};