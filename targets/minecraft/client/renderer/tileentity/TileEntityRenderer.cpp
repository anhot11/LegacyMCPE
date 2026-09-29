#include "TileEntityRenderer.h"

#include "TileEntityRenderDispatcher.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/resources/ResourceLocation.h"

void TileEntityRenderer::bindTexture(ResourceLocation* location) {
    Textures* t = tileEntityRenderDispatcher->textures;
    if (t != nullptr) t->bind(t->loadTexture(location->getTexture()));
}

void TileEntityRenderer::bindTexture(const std::string& urlTexture,
                                     ResourceLocation* location) {
    Textures* t = tileEntityRenderDispatcher->textures;
    if (t != nullptr)
        t->bind(t->loadHttpTexture(urlTexture, location->getTexture()));
}

Level* TileEntityRenderer::getLevel() {
    return tileEntityRenderDispatcher->level;
}

void TileEntityRenderer::init(
    TileEntityRenderDispatcher* tileEntityRenderDispatcher) {
    this->tileEntityRenderDispatcher = tileEntityRenderDispatcher;
}

Font* TileEntityRenderer::getFont() {
    return tileEntityRenderDispatcher->getFont();
}