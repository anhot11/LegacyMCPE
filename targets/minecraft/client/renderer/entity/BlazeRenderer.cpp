#include "BlazeRenderer.h"

#include <memory>

#include "minecraft/client/model/BlazeModel.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/renderer/entity/MobRenderer.h"
#include "minecraft/client/resources/ResourceLocation.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/monster/Blaze.h"

ResourceLocation BlazeRenderer::BLAZE_LOCATION = ResourceLocation(TN_MOB_BLAZE);

BlazeRenderer::BlazeRenderer() : MobRenderer(new BlazeModel(), 0.5f) {
    modelVersion = ((BlazeModel*)model)->modelVersion();
}

void BlazeRenderer::render(std::shared_ptr<Entity> _mob, double x, double y,
                           double z, float rot, float a) {
    // 4J - original version used generics and thus had an input parameter of
    // type Blaze rather than shared_ptr<Entity>  we have here - do some casting
    // around instead
    std::shared_ptr<Blaze> mob = std::dynamic_pointer_cast<Blaze>(_mob);

    int modelVersion = ((BlazeModel*)model)->modelVersion();
    if (modelVersion != this->modelVersion) {
        this->modelVersion = modelVersion;
        model = new BlazeModel();
    }
    MobRenderer::render(mob, x, y, z, rot, a);
}

ResourceLocation* BlazeRenderer::getTextureLocation(
    std::shared_ptr<Entity> mob) {
    return &BLAZE_LOCATION;
}