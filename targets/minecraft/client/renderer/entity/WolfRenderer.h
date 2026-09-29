#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class WolfRenderer : public MobRenderer {
private:
    static ResourceLocation* WOLF_LOCATION;
    static ResourceLocation* WOLF_TAME_LOCATION;
    static ResourceLocation* WOLF_ANGRY_LOCATION;
    static ResourceLocation* WOLF_COLLAR_LOCATION;

public:
    WolfRenderer(Model* model, Model* armor, float shadow);

protected:
    virtual float getBob(std::shared_ptr<LivingEntity> _mob, float a);
    virtual int prepareArmor(std::shared_ptr<LivingEntity> mob, int layer,
                             float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};