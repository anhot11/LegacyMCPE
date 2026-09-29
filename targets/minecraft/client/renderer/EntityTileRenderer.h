#pragma once

#include <memory>

class ChestTileEntity;
class EnderChestTileEntity;
class Tile;

class EntityTileRenderer {
public:
    static EntityTileRenderer* instance;

private:
    std::shared_ptr<ChestTileEntity> chest;
    std::shared_ptr<ChestTileEntity> trappedChest;
    std::shared_ptr<EnderChestTileEntity> enderChest;

public:
    EntityTileRenderer();
    void render(Tile* tile, int data, float brightness, float alpha,
                bool setColor = true,
                bool useCompiled =
                    true);  // 4J - added setColor parameter and alpha for chest
                            // in the crafting menu, and added useCompiled
};
