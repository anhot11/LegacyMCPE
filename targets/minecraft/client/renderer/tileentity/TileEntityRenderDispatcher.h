#pragma once
#include <memory>
#include <unordered_map>

#include "java/Class.h"
#include "java/JavaIntHash.h"
#include "minecraft/client/renderer/Textures.h"

class Mob;
class Level;
class TileEntityRenderer;
class TileEntity;
class Font;
class LivingEntity;
class Textures;

class TileEntityRenderDispatcher {
public:
    static void staticCtor();  // 4J added

private:
    typedef std::unordered_map<eINSTANCEOF, TileEntityRenderer*,
                               eINSTANCEOFKeyHash, eINSTANCEOFKeyEq>
        classToTileRendererMap;
    classToTileRendererMap renderers;

public:
    static TileEntityRenderDispatcher* instance;

private:
    Font* font;

public:
    static double xOff, yOff, zOff;

    Textures* textures;
    Level* level;
    std::shared_ptr<LivingEntity> cameraEntity;
    float playerRotY;
    float playerRotX;
    double xPlayer, yPlayer, zPlayer;

private:
    TileEntityRenderDispatcher();

public:
    TileEntityRenderer* getRenderer(eINSTANCEOF e);
    bool hasRenderer(std::shared_ptr<TileEntity> e);
    TileEntityRenderer* getRenderer(std::shared_ptr<TileEntity> e);
    void prepare(Level* level, Textures* textures, Font* font,
                 std::shared_ptr<LivingEntity> player, float a);
    void render(std::shared_ptr<TileEntity> e, float a, bool setColor = true);
    void render(std::shared_ptr<TileEntity> entity, double x, double y,
                double z, float a, bool setColor = true, float alpha = 1.0f,
                bool useCompiled = true);  // 4J Added useCompiled
    void setLevel(Level* level);
    double distanceToSqr(double x, double y, double z);
    Font* getFont();
};
