#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class MushroomCowRenderer : public MobRenderer {
private:
    static ResourceLocation MOOSHROOM_LOCATION;

public:
    MushroomCowRenderer(Model* model, float shadow);

    virtual void render(std::shared_ptr<Entity> _mob, double x, double y,
                        double z, float rot, float a);

protected:
    virtual void additionalRendering(std::shared_ptr<LivingEntity> _mob,
                                     float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};