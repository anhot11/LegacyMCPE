#pragma once
#include "EntityRenderer.h"

class TileRenderer;

class TntRenderer : public EntityRenderer {
private:
    TileRenderer* renderer;

public:
    TntRenderer();
    virtual void render(std::shared_ptr<Entity> _tnt, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};