#pragma once
#include "EntityRenderer.h"

class LeashKnotModel;
class ResourceLocation;

class LeashKnotRenderer : public EntityRenderer {
private:
    static ResourceLocation KNOT_LOCATION;
    LeashKnotModel* model;

public:
    LeashKnotRenderer();
    ~LeashKnotRenderer();
    virtual void render(std::shared_ptr<Entity> entity, double x, double y,
                        double z, float rot, float a);

protected:
    virtual ResourceLocation* getTextureLocation(
        std::shared_ptr<Entity> entity);
};