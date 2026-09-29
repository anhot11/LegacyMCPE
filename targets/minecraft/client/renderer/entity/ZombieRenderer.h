#pragma once
#include <memory>

#include "HumanoidMobRenderer.h"

class VillagerZombieModel;
class Zombie;
class HumanoidModel;
class ResourceLocation;

class ZombieRenderer : public HumanoidMobRenderer {
private:
    static ResourceLocation ZOMBIE_PIGMAN_LOCATION;
    static ResourceLocation ZOMBIE_LOCATION;
    static ResourceLocation ZOMBIE_VILLAGER_LOCATION;

    HumanoidModel* defaultModel;
    VillagerZombieModel* villagerModel;

protected:
    HumanoidModel* defaultArmorParts1;
    HumanoidModel* defaultArmorParts2;
    HumanoidModel* villagerArmorParts1;
    HumanoidModel* villagerArmorParts2;

private:
    int modelVersion;

public:
    ZombieRenderer();

protected:
    virtual void createArmorParts();
    virtual int prepareArmor(std::shared_ptr<LivingEntity> _mob, int layer,
                             float a);

public:
    virtual void render(std::shared_ptr<Entity> _mob, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(
        std::shared_ptr<Entity> entity);

protected:
    virtual void additionalRendering(std::shared_ptr<LivingEntity> _mob,
                                     float a);

private:
    virtual void swapArmor(std::shared_ptr<Zombie> mob);

protected:
    virtual void setupRotations(std::shared_ptr<LivingEntity> _mob, float bob,
                                float bodyRot, float a);
};