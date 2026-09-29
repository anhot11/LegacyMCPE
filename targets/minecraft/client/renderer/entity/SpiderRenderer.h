#pragma once
#include "MobRenderer.h"

class ResourceLocation;

class SpiderRenderer : public MobRenderer {
private:
    static ResourceLocation SPIDER_LOCATION;
    static ResourceLocation SPIDER_EYES_LOCATION;

public:
    SpiderRenderer();

protected:
    virtual float getFlipDegrees(std::shared_ptr<LivingEntity> spider);
    virtual int prepareArmor(std::shared_ptr<LivingEntity> _spider, int layer,
                             float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};