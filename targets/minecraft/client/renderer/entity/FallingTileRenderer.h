#pragma once
#include "EntityRenderer.h"

class TileRenderer;

class FallingTileRenderer : public EntityRenderer {
private:
    TileRenderer* tileRenderer;

public:
    FallingTileRenderer();

    virtual void render(std::shared_ptr<Entity> _tile, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};