#pragma once
#include <memory>

#include "EntityRenderer.h"

class Minecart;
class Model;
class ResourceLocation;
class Tile;
class TileRenderer;

class MinecartRenderer : public EntityRenderer {
private:
    static ResourceLocation MINECART_LOCATION;

protected:
    Model* model;
    TileRenderer* renderer;

public:
    MinecartRenderer();
    virtual void render(std::shared_ptr<Entity> _cart, double x, double y,
                        double z, float rot, float a);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);

protected:
    virtual void renderMinecartContents(std::shared_ptr<Minecart> cart, float a,
                                        Tile* tile, int tileData);
};