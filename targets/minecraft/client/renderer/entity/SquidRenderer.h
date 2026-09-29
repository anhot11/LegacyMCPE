#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class SquidRenderer : public MobRenderer {
private:
    static ResourceLocation SQUID_LOCATION;

public:
    SquidRenderer(Model* model, float shadow);
    virtual void render(std::shared_ptr<Entity> mob, double x, double y,
                        double z, float rot, float a);

protected:
    virtual void setupRotations(std::shared_ptr<LivingEntity> _mob, float bob,
                                float bodyRot, float a);
    virtual float getBob(std::shared_ptr<LivingEntity> _mob, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};