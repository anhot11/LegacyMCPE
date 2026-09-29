#pragma once
#include "MobRenderer.h"

class ResourceLocation;

class BlazeRenderer : public MobRenderer {
private:
    static ResourceLocation BLAZE_LOCATION;
    int modelVersion;

public:
    BlazeRenderer();

    virtual void render(std::shared_ptr<Entity> mob, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};