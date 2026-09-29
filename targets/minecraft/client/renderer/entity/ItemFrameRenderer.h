#pragma once
#include <memory>

#include "EntityRenderer.h"

class Icon;
class ItemFrame;
class ResourceLocation;

class ItemFrameRenderer : public EntityRenderer {
private:
    static ResourceLocation MAP_BACKGROUND_LOCATION;
    Icon* backTexture;

public:
    void registerTerrainTextures(IconRegister* iconRegister);
    virtual void render(std::shared_ptr<Entity> _itemframe, double x, double y,
                        double z, float rot, float a);

private:
    void drawFrame(std::shared_ptr<ItemFrame> itemFrame);
    void drawItem(std::shared_ptr<ItemFrame> entity);
};
