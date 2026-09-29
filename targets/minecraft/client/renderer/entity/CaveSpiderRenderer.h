#pragma once
#include "SpiderRenderer.h"

class CaveSpider;
class ResourceLocation;

class CaveSpiderRenderer : public SpiderRenderer {
private:
    static ResourceLocation CAVE_SPIDER_LOCATION;
    static float s_scale;

public:
    CaveSpiderRenderer();

protected:
    virtual void scale(std::shared_ptr<LivingEntity> mob, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};