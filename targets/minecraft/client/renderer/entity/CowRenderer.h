#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class CowRenderer : public MobRenderer {
private:
    static ResourceLocation COW_LOCATION;

public:
    CowRenderer(Model* model, float shadow);

    virtual void render(std::shared_ptr<Entity> _mob, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};