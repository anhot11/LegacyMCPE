#pragma once
#include "MobRenderer.h"

class ResourceLocation;

class LavaSlimeRenderer : public MobRenderer {
private:
    int modelVersion;
    static ResourceLocation MAGMACUBE_LOCATION;

public:
    LavaSlimeRenderer();
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);

protected:
    virtual void scale(std::shared_ptr<LivingEntity> _slime, float a);
};