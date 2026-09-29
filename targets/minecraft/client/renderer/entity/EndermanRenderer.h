#pragma once
#include "MobRenderer.h"
#include "java/Random.h"

class EnderMan;
class EndermanModel;
class ResourceLocation;

class EndermanRenderer : public MobRenderer {
private:
    EndermanModel* model;
    Random random;
    static ResourceLocation ENDERMAN_EYES_LOCATION;
    static ResourceLocation ENDERMAN_LOCATION;

public:
    EndermanRenderer();

    void render(std::shared_ptr<Entity> _mob, double x, double y, double z,
                float rot, float a);
    ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
    void additionalRendering(std::shared_ptr<LivingEntity> _mob, float a);

protected:
    int prepareArmor(std::shared_ptr<LivingEntity> _mob, int layer, float a);
};