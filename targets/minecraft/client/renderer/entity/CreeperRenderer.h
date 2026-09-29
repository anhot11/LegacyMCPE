#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class CreeperRenderer : public MobRenderer {
private:
    static ResourceLocation POWER_LOCATION;
    static ResourceLocation CREEPER_LOCATION;
    Model* armorModel;

public:
    CreeperRenderer();

protected:
    virtual void scale(std::shared_ptr<LivingEntity> _mob, float a);
    virtual int getOverlayColor(std::shared_ptr<LivingEntity> mob, float br,
                                float a);
    virtual int prepareArmor(std::shared_ptr<LivingEntity> mob, int layer,
                             float a);
    virtual int prepareArmorOverlay(std::shared_ptr<LivingEntity> _mob,
                                    int layer, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};