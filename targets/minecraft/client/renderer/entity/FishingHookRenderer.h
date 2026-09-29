#pragma once
#include "EntityRenderer.h"

class ResourceLocation;

class FishingHookRenderer : public EntityRenderer {
private:
    static ResourceLocation PARTICLE_LOCATION;

public:
    virtual void render(std::shared_ptr<Entity> _hook, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};