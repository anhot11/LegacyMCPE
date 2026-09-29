#pragma once
#include <map>
#include <memory>
#include <string>

#include "MobRenderer.h"
#include "minecraft/client/resources/ResourceLocation.h"

class EntityHorse;
class PathfinderMob;
class Model;
class ResourceLocation;

class HorseRenderer : public MobRenderer {
private:
    static std::map<std::string, ResourceLocation*> LAYERED_LOCATION_CACHE;

    static ResourceLocation HORSE_LOCATION;
    static ResourceLocation HORSE_MULE_LOCATION;
    static ResourceLocation HORSE_DONKEY_LOCATION;
    static ResourceLocation HORSE_ZOMBIE_LOCATION;
    static ResourceLocation HORSE_SKELETON_LOCATION;

public:
    HorseRenderer(Model* model, float f);

protected:
    void adjustHeight(std::shared_ptr<PathfinderMob> mob, float FHeight);
    virtual void scale(std::shared_ptr<LivingEntity> entityliving, float f);
    virtual void renderModel(std::shared_ptr<LivingEntity> mob, float wp,
                             float ws, float bob, float headRotMinusBodyRot,
                             float headRotx, float scale);
    virtual void bindTexture(ResourceLocation* location);
    virtual ResourceLocation* getTextureLocation(
        std::shared_ptr<Entity> entity);

private:
    ResourceLocation* getOrCreateLayeredTextureLocation(
        std::shared_ptr<EntityHorse> horse);
};