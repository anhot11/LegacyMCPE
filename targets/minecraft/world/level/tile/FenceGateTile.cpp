#include "FenceGateTile.h"

#include <optional>

#include "LevelEvent.h"
#include "minecraft/Direction.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LevelSource.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/DirectionalTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/AABB.h"

FenceGateTile::FenceGateTile(int id)
    : DirectionalTile(id, Material::wood, false) {}

Icon* FenceGateTile::getTexture(int face, int data) {
    return Tile::wood->getTexture(face);
}

bool FenceGateTile::mayPlace(Level* level, int x, int y, int z) {
    if (!level->getMaterial(x, y - 1, z)->isSolid()) return false;
    return Tile::mayPlace(level, x, y, z);
}

std::optional<AABB> FenceGateTile::getAABB(Level* level, int x, int y, int z) {
    int data = level->getData(x, y, z);
    if (isOpen(data)) {
        return std::nullopt;
    }

    switch (data) {
        case Direction::NORTH:
        case Direction::SOUTH:
            return AABB{static_cast<double>(x),
                        static_cast<double>(y),
                        z + 6.0 / 16.0,
                        x + 1.0,
                        y + 1.5,
                        z + 10.0 / 16.0};
        default:
            return AABB{x + 6.0 / 16.0,
                        static_cast<double>(y),
                        static_cast<double>(z),
                        x + 10.0 / 16.0,
                        y + 1.5,
                        z + 1.0};
    }
}

// 4J - Brought forward from 1.2.3 to fix hit box rotation
void FenceGateTile::updateShape(
    LevelSource* level, int x, int y, int z, int forceData,
    std::shared_ptr<TileEntity>
        forceEntity)  // 4J added forceData, forceEntity param
{
    int data = getDirection(level->getData(x, y, z));
    if (data == Direction::NORTH || data == Direction::SOUTH) {
        setShape(0, 0, 6.0f / 16.0f, 1, 1.0f, 10.0f / 16.0f);
    } else {
        setShape(6.0f / 16.0f, 0, 0, 10.0f / 16.0f, 1.0f, 1);
    }
}

bool FenceGateTile::blocksLight() { return false; }

bool FenceGateTile::isSolidRender(bool isServerLevel) { return false; }

bool FenceGateTile::isCubeShaped() { return false; }

bool FenceGateTile::isPathfindable(LevelSource* level, int x, int y, int z) {
    return isOpen(level->getData(x, y, z));
}

int FenceGateTile::getRenderShape() { return Tile::SHAPE_FENCE_GATE; }

void FenceGateTile::setPlacedBy(Level* level, int x, int y, int z,
                                std::shared_ptr<LivingEntity> by,
                                std::shared_ptr<ItemInstance> itemInstance) {
    int dir = (((Mth::floor(by->yRot * 4 / (360) + 0.5)) & 3)) % 4;
    level->setData(x, y, z, dir, Tile::UPDATE_CLIENTS);
}

bool FenceGateTile::use(Level* level, int x, int y, int z,
                        std::shared_ptr<Player> player, int clickedFace,
                        float clickX, float clickY, float clickZ,
                        bool soundOnly /*=false*/)  // 4J added soundOnly param
{
    if (soundOnly) {
        // 4J - added - just do enough to play the sound
        level->levelEvent(
            player, LevelEvent::SOUND_OPEN_DOOR, x, y, z,
            0);  // 4J - changed event to pass player rather than nullptr as the
                 // source of the event so we can filter the broadcast properly
        return false;
    }

    int data = level->getData(x, y, z);
    if (isOpen(data)) {
        level->setData(x, y, z, data & ~OPEN_BIT, Tile::UPDATE_CLIENTS);
    } else {
        // open the door from the player
        int dir = (((Mth::floor(player->yRot * 4 / (360) + 0.5)) & 3)) % 4;
        int current = getDirection(data);
        if (current == ((dir + 2) % 4)) {
            data = dir;
        }
        level->setData(x, y, z, data | OPEN_BIT, Tile::UPDATE_CLIENTS);
    }
    level->levelEvent(player, LevelEvent::SOUND_OPEN_DOOR, x, y, z, 0);
    return true;
}

void FenceGateTile::neighborChanged(Level* level, int x, int y, int z,
                                    int type) {
    if (level->isClientSide) return;

    int data = level->getData(x, y, z);

    bool signal = level->hasNeighborSignal(x, y, z);
    if (signal ||
        ((type > 0 && Tile::tiles[type]->isSignalSource()) || type == 0)) {
        if (signal && !isOpen(data)) {
            level->setData(x, y, z, data | OPEN_BIT, Tile::UPDATE_CLIENTS);
            level->levelEvent(nullptr, LevelEvent::SOUND_OPEN_DOOR, x, y, z, 0);
        } else if (!signal && isOpen(data)) {
            level->setData(x, y, z, data & ~OPEN_BIT, Tile::UPDATE_CLIENTS);
            level->levelEvent(nullptr, LevelEvent::SOUND_OPEN_DOOR, x, y, z, 0);
        }
    }
}

bool FenceGateTile::isOpen(int data) { return (data & OPEN_BIT) != 0; }

void FenceGateTile::registerIcons(IconRegister* iconRegister) {
    // None
}

bool FenceGateTile::shouldRenderFace(LevelSource* level, int x, int y, int z,
                                     int face) {
    return true;
}
