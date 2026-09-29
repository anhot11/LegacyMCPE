#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class SlimeRenderer : public MobRenderer {
private:
    Model* armor;
    static ResourceLocation SLIME_LOCATION;

public:
    SlimeRenderer(Model* model, Model* armor, float shadow);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);

protected:
    virtual int prepareArmor(std::shared_ptr<LivingEntity> _slime, int layer,
                             float a);
    virtual void scale(std::shared_ptr<LivingEntity> _slime, float a);
};