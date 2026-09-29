#pragma once
#include <memory>
#include <string>

#include "minecraft/client/model/geom/Model.h"
#include "minecraft/client/renderer/ItemInHandRenderer.h"
#include "minecraft/client/renderer/Tesselator.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/renderer/TileRenderer.h"
#include "minecraft/client/resources/ResourceLocation.h"

class Tile;
class Entity;
class Level;
class AABB;
class IconRegister;
class ResourceLocation;
class EntityRenderDispatcher;
class Font;
class Model;
class TileRenderer;

// 4J - this was originally a generic of type EntityRenderer<T extends Entity>
class EntityRenderer {
    friend class PlayerRenderer;  // 4J Added to allow PlayerRenderer to call
                                  // renderShadow
protected:
    EntityRenderDispatcher* entityRenderDispatcher;

private:
    static ResourceLocation SHADOW_LOCATION;

protected:
    Model* model;  // TODO 4J: Check why exactly this is here, it seems to get
                   // shadowed by classes inheriting from this by their own

protected:
    TileRenderer* tileRenderer;  // 4J - changed to protected so derived classes
                                 // can use instead of shadowing their own

protected:
    float shadowRadius;
    float shadowStrength;

public:
    EntityRenderer();  // 4J - added
    virtual ~EntityRenderer();

public:
    virtual void render(std::shared_ptr<Entity> entity, double x, double y,
                        double z, float rot, float a) = 0;

protected:
    virtual void bindTexture(std::shared_ptr<Entity> entity);
    virtual void bindTexture(ResourceLocation* location);
    virtual bool bindTexture(const std::string& urlTexture, int backupTexture);
    virtual bool bindTexture(const std::string& urlTexture,
                             const std::string& backupTexture);

    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);

private:
    virtual void renderFlame(std::shared_ptr<Entity> e, double x, double y,
                             double z, float a);
    virtual void renderShadow(std::shared_ptr<Entity> e, double x, double y,
                              double z, float pow, float a);

    virtual Level* getLevel();
    virtual void renderTileShadow(Tile* tt, double x, double y, double z,
                                  int xt, int yt, int zt, float pow, float r,
                                  double xo, double yo, double zo);

public:
    virtual void render(AABB* bb, double xo, double yo, double zo);
    static void renderFlat(AABB* bb);
    static void renderFlat(float x0, float y0, float z0, float x1, float y1,
                           float z1);
    virtual void init(EntityRenderDispatcher* entityRenderDispatcher);
    virtual void postRender(std::shared_ptr<Entity> entity, double x, double y,
                            double z, float rot, float a,
                            bool bRenderPlayerShadow);
    virtual Font* getFont();
    virtual void registerTerrainTextures(IconRegister* iconRegister);

public:
    // 4J Added
    virtual Model* getModel() { return model; }
    virtual void SetItemFrame(bool bSet) {}
};
