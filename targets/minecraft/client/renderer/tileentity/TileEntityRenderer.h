#pragma once
#include <memory>
#include <string>

#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/resources/ResourceLocation.h"

class TileEntityRenderDispatcher;
class TileEntity;
class Level;
class Font;
class ResourceLocation;

class TileEntityRenderer {
protected:
    TileEntityRenderDispatcher* tileEntityRenderDispatcher;

public:
    virtual void render(std::shared_ptr<TileEntity> entity, double x, double y,
                        double z, float a, bool setColor, float alpha,
                        bool useCompiled) = 0;  // 4J added setColor param,
                                                // alpha and useCompiled
    virtual void onNewLevel(Level* level) {}

protected:
    void bindTexture(
        ResourceLocation* location);  // 4J - changed from std::string to int
    void bindTexture(
        const std::string& urlTexture,
        ResourceLocation* location);  // 4J - changed from std::string to int
private:
    Level* getLevel();

public:
    virtual void init(TileEntityRenderDispatcher* tileEntityRenderDispatcher);
    Font* getFont();
};
