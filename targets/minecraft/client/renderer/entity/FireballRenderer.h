#pragma once
#include "EntityRenderer.h"

class FireballRenderer : public EntityRenderer {
private:
    float scale;

public:
    FireballRenderer(float scale);

    virtual void render(std::shared_ptr<Entity> _fireball, double x, double y,
                        double z, float rot, float a);

private:
    // 4J Added override
    virtual void renderFlame(std::shared_ptr<Entity> entity, double x, double y,
                             double z, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};
