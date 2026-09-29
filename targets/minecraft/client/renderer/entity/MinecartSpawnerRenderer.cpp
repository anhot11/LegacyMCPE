#include "MinecartSpawnerRenderer.h"

#include "minecraft/client/renderer/entity/MinecartRenderer.h"
#include "minecraft/client/renderer/tileentity/MobSpawnerRenderer.h"
#include "minecraft/world/entity/item/MinecartSpawner.h"
#include "minecraft/world/level/tile/Tile.h"

void MinecartSpawnerRenderer::renderMinecartContents(
    std::shared_ptr<MinecartSpawner> cart, float a, Tile* tile, int tileData) {
    MinecartRenderer::renderMinecartContents(cart, a, tile, tileData);

    if (tile == Tile::mobSpawner) {
        MobSpawnerRenderer::render(cart->getSpawner(), cart->x, cart->y,
                                   cart->z, a);
    }
}