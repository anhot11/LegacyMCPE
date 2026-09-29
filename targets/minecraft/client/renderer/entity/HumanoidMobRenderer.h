#pragma once
#include <map>
#include <memory>
#include <string>

#include "MobRenderer.h"

class HumanoidModel;
class Giant;
class ArmorItem;
class Entity;
class ItemInstance;
class ResourceLocation;

class HumanoidMobRenderer : public MobRenderer {
private:
    static const std::string MATERIAL_NAMES[5];
    static std::map<std::string, ResourceLocation> ARMOR_LOCATION_CACHE;

protected:
    HumanoidModel* humanoidModel;
    float _scale;
    HumanoidModel* armorParts1;
    HumanoidModel* armorParts2;

    void _init(HumanoidModel* humanoidModel, float scale);

public:
    static ResourceLocation* getArmorLocation(ArmorItem* armorItem, int layer);
    static ResourceLocation* getArmorLocation(ArmorItem* armorItem, int layer,
                                              bool overlay);

    HumanoidMobRenderer(HumanoidModel* humanoidModel, float shadow);
    HumanoidMobRenderer(HumanoidModel* humanoidModel, float shadow,
                        float scale);

    virtual void prepareSecondPassArmor(std::shared_ptr<LivingEntity> mob,
                                        int layer, float a);

protected:
    virtual void createArmorParts();
    virtual int prepareArmor(std::shared_ptr<LivingEntity> _mob, int layer,
                             float a);
    virtual void render(std::shared_ptr<Entity> _mob, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
    virtual void prepareCarriedItem(std::shared_ptr<Entity> mob,
                                    std::shared_ptr<ItemInstance> item);
    virtual void additionalRendering(std::shared_ptr<LivingEntity> mob,
                                     float a);
    virtual void scale(std::shared_ptr<LivingEntity> mob, float a);
};