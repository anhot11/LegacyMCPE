#pragma once
#include "EntityRenderer.h"

class ResourceLocation;

class ArrowRenderer : public EntityRenderer {
private:
    static ResourceLocation ARROW_LOCATION;

public:
    virtual void render(std::shared_ptr<Entity> _arrow, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};
