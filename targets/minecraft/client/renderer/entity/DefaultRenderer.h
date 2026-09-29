#pragma once
#include <memory>

#include "EntityRenderer.h"

class DefaultRenderer : public EntityRenderer {
public:
    virtual void render(std::shared_ptr<Entity> entity, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob) {
        return nullptr;
    };
};