#pragma once
#include "MobRenderer.h"

class WitchModel;
class ResourceLocation;

class WitchRenderer : public MobRenderer {
private:
    static ResourceLocation WITCH_LOCATION;
    WitchModel* witchModel;

public:
    WitchRenderer();
    virtual void render(std::shared_ptr<Entity> entity, double x, double y,
                        double z, float rot, float a);

protected:
    virtual ResourceLocation* getTextureLocation(
        std::shared_ptr<Entity> entity);
    virtual void additionalRendering(std::shared_ptr<LivingEntity> mob,
                                     float a);
    virtual void translateWeaponItem();
    virtual void scale(std::shared_ptr<LivingEntity> mob, float a);
};