#pragma once
#include <memory>
#include <string>

#include "EntityRenderer.h"
#include "minecraft/client/resources/ResourceLocation.h"
#include "minecraft/world/entity/LivingEntity.h"

class LivingEntity;
class Model;
class ResourceLocation;

class LivingEntityRenderer : public EntityRenderer {
    static const int PLAYER_NAME_READABLE_FULLSCREEN = 16;
    static const int PLAYER_NAME_READABLE_DISTANCE_SPLITSCREEN = 8;
    static const int PLAYER_NAME_READABLE_DISTANCE_SD = 8;

    static ResourceLocation ENCHANT_GLINT_LOCATION;
    static int MAX_ARMOR_LAYERS;

protected:
    // Model *model; // 4J Stu - This shadows the one in EntityRenderer
    Model* armor;

public:
    LivingEntityRenderer(Model* model, float shadow);
    virtual void render(std::shared_ptr<Entity> mob, double x, double y,
                        double z, float rot, float a);
    virtual void setArmor(Model* armor);

private:
    float rotlerp(float from, float to, float a);

protected:
    virtual void renderModel(std::shared_ptr<LivingEntity> mob, float wp,
                             float ws, float bob, float headRotMinusBodyRot,
                             float headRotx, float scale);
    virtual void setupPosition(std::shared_ptr<LivingEntity> mob, double x,
                               double y, double z);
    virtual void setupRotations(std::shared_ptr<LivingEntity> mob, float bob,
                                float bodyRot, float a);
    virtual float getAttackAnim(std::shared_ptr<LivingEntity> mob, float a);
    virtual float getBob(std::shared_ptr<LivingEntity> mob, float a);
    virtual void additionalRendering(std::shared_ptr<LivingEntity> mob,
                                     float a);
    virtual void renderArrows(std::shared_ptr<LivingEntity> mob, float a);
    virtual int prepareArmorOverlay(std::shared_ptr<LivingEntity> mob,
                                    int layer, float a);
    virtual int prepareArmor(std::shared_ptr<LivingEntity> mob, int layer,
                             float a);
    virtual void prepareSecondPassArmor(std::shared_ptr<LivingEntity> mob,
                                        int layer, float a);
    virtual float getFlipDegrees(std::shared_ptr<LivingEntity> mob);
    virtual int getOverlayColor(std::shared_ptr<LivingEntity> mob, float br,
                                float a);
    virtual void scale(std::shared_ptr<LivingEntity> mob, float a);
    virtual void renderName(std::shared_ptr<LivingEntity> mob, double x,
                            double y, double z);
    virtual bool shouldShowName(std::shared_ptr<LivingEntity> mob);
    virtual void renderNameTags(std::shared_ptr<LivingEntity> mob, double x,
                                double y, double z, const std::string& msg,
                                float scale, double dist);
    virtual void renderNameTag(std::shared_ptr<LivingEntity> mob,
                               const std::string& name, double x, double y,
                               double z, int maxDist, int color = 0xff000000);
};