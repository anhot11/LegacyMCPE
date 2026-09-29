#include "SoulSandTile.h"

#include <memory>
#include <optional>

#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/AABB.h"

SoulSandTile::SoulSandTile(int id) : Tile(id, Material::sand) {}

std::optional<AABB> SoulSandTile::getAABB(Level* level, int x, int y, int z) {
    float r = 2 / 16.0f;
    return AABB(x, y, z, x + 1, y + 1 - r, z + 1);
}

void SoulSandTile::entityInside(Level* level, int x, int y, int z,
                                std::shared_ptr<Entity> entity) {
    entity->xd *= 0.4;
    entity->zd *= 0.4;
}
