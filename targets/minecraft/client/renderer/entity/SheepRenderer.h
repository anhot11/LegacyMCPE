#pragma once
#include "MobRenderer.h"

class Model;
class ResourceLocation;

class SheepRenderer : public MobRenderer {
private:
    static ResourceLocation SHEEP_LOCATION;
    static ResourceLocation SHEEP_FUR_LOCATION;

public:
    SheepRenderer(Model* model, Model* armor, float shadow);

protected:
    virtual int prepareArmor(std::shared_ptr<LivingEntity> _sheep, int layer,
                             float a);

public:
    virtual void render(std::shared_ptr<Entity> mob, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};