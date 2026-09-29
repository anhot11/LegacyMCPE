#pragma once

#include "MobRenderer.h"

class VillagerGolemModel;
class ResourceLocation;

class VillagerGolemRenderer : public MobRenderer {
private:
    VillagerGolemModel* golemModel;
    static ResourceLocation GOLEM_LOCATION;

public:
    VillagerGolemRenderer();
    virtual void render(std::shared_ptr<Entity> mob, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);

protected:
    virtual void setupRotations(std::shared_ptr<LivingEntity> _mob, float bob,
                                float bodyRot, float a);
    virtual void additionalRendering(std::shared_ptr<LivingEntity> mob,
                                     float a);
};