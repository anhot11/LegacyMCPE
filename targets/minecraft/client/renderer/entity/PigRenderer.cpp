#include "PigRenderer.h"

#include <memory>

#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/renderer/entity/MobRenderer.h"
#include "minecraft/client/resources/ResourceLocation.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/animal/Pig.h"

class Model;

ResourceLocation PigRenderer::PIG_LOCATION = ResourceLocation(TN_MOB_PIG);
ResourceLocation PigRenderer::SADDLE_LOCATION = ResourceLocation(TN_MOB_SADDLE);

PigRenderer::PigRenderer(Model* model, Model* armor, float shadow)
    : MobRenderer(model, shadow) {
    setArmor(armor);
}

int PigRenderer::prepareArmor(std::shared_ptr<LivingEntity> _pig, int layer,
                              float a) {
    // 4J - dynamic cast required because we aren't using templates/generics in
    // our version
    std::shared_ptr<Pig> pig = std::dynamic_pointer_cast<Pig>(_pig);

    if (layer == 0 && pig->hasSaddle()) {
        bindTexture(&SADDLE_LOCATION);

        return 1;
    }

    return -1;
}

void PigRenderer::render(std::shared_ptr<Entity> mob, double x, double y,
                         double z, float rot, float a) {
    MobRenderer::render(mob, x, y, z, rot, a);
}

ResourceLocation* PigRenderer::getTextureLocation(std::shared_ptr<Entity> mob) {
    return &PIG_LOCATION;
}