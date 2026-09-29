#pragma once
#include "MobRenderer.h"

class Silverfish;
class ResourceLocation;

class SilverfishRenderer : public MobRenderer {
private:
    // int modelVersion;
    static ResourceLocation SILVERFISH_LOCATION;

public:
    SilverfishRenderer();

protected:
    float getFlipDegrees(std::shared_ptr<LivingEntity> spider);

public:
    virtual void render(std::shared_ptr<Entity> _mob, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);

protected:
    virtual int prepareArmor(std::shared_ptr<LivingEntity> _silverfish,
                             int layer, float a);
};
