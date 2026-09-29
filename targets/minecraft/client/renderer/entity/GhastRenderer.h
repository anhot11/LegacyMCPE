#pragma once
#include "MobRenderer.h"

class ResourceLocation;

class GhastRenderer : public MobRenderer {
private:
    static ResourceLocation GHAST_LOCATION;
    static ResourceLocation GHAST_SHOOTING_LOCATION;

public:
    GhastRenderer();

protected:
    virtual void scale(std::shared_ptr<LivingEntity> mob, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};