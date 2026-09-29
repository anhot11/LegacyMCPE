#include "EntityTileRenderer.h"

#include "minecraft/client/renderer/tileentity/TileEntityRenderDispatcher.h"
#include "minecraft/world/level/tile/ChestTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/ChestTileEntity.h"
#include "minecraft/world/level/tile/entity/EnderChestTileEntity.h"

EntityTileRenderer* EntityTileRenderer::instance = new EntityTileRenderer;

EntityTileRenderer::EntityTileRenderer() {
    chest = std::make_shared<ChestTileEntity>();
    trappedChest = std::shared_ptr<ChestTileEntity>(
        new ChestTileEntity(ChestTile::TYPE_TRAP));
    enderChest = std::make_shared<EnderChestTileEntity>();
}

void EntityTileRenderer::render(Tile* tile, int data, float brightness,
                                float alpha, bool setColor, bool useCompiled) {
    if (tile->id == Tile::enderChest_Id) {
        TileEntityRenderDispatcher::instance->render(
            enderChest, 0, 0, 0, 0, setColor, alpha, useCompiled);
    } else if (tile->id == Tile::chest_trap_Id) {
        TileEntityRenderDispatcher::instance->render(
            trappedChest, 0, 0, 0, 0, setColor, alpha, useCompiled);
    } else {
        TileEntityRenderDispatcher::instance->render(
            chest, 0, 0, 0, 0, setColor, alpha, useCompiled);
    }
}
