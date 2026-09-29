#include "WebTile.h"

#include <memory>
#include <optional>

#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/AABB.h"

WebTile::WebTile(int id) : Tile(id, Material::web) {}

void WebTile::entityInside(Level* level, int x, int y, int z,
                           std::shared_ptr<Entity> entity) {
    entity->makeStuckInWeb();
}

bool WebTile::isSolidRender(bool isServerLevel) { return false; }

std::optional<AABB> WebTile::getAABB(Level* level, int x, int y, int z) {
    return std::nullopt;
}

int WebTile::getRenderShape() { return Tile::SHAPE_CROSS_TEXTURE; }

bool WebTile::blocksLight() { return false; }

bool WebTile::isCubeShaped() { return false; }

int WebTile::getResource(int data, Random* random, int playerBonusLevel) {
    // @TODO: Explosives currently also give string back. Fix?
    return Item::string->id;
}

bool WebTile::isSilkTouchable() { return true; }
