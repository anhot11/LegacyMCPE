#pragma once
#include <memory>

#include "LivingEntityRenderer.h"

class Mob;
class Model;

// This was used in MobRenderer but lots of code moved to LivingEntity and I
// haven't put this back yet
/*#define PLAYER_NAME_READABLE_FULLSCREEN 16
#define PLAYER_NAME_READABLE_DISTANCE_SPLITSCREEN 8
#define PLAYER_NAME_READABLE_DISTANCE_SD 8*/

// 4J - this used to be a generic : public class MobRenderer<T extends Mob>
// extends EntityRenderer<T>
class MobRenderer : public LivingEntityRenderer {
public:
    MobRenderer(Model* model, float shadow);
    virtual void render(std::shared_ptr<Entity> mob, double x, double y,
                        double z, float rot, float a);

protected:
    virtual bool shouldShowName(std::shared_ptr<LivingEntity> mob);
    virtual void renderLeash(std::shared_ptr<Mob> entity, double x, double y,
                             double z, float rot, float a);

private:
    double lerp(double prev, double next, double a);
};
