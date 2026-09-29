#include "TheEndPortalFrameTile.h"

#include <memory>

#include "minecraft/Facing.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/redstone/Redstone.h"
#include "minecraft/world/level/tile/Tile.h"

class Icon;

const std::string TheEndPortalFrameTile::TEXTURE_EYE = "endframe_eye";

TheEndPortalFrameTile::TheEndPortalFrameTile(int id)
    : Tile(id, Material::glass, false) {
    iconTop = nullptr;
    iconEye = nullptr;
}

Icon* TheEndPortalFrameTile::getTexture(int face, int data) {
    if (face == Facing::UP) {
        return iconTop;
    }
    if (face == Facing::DOWN) {
        return Tile::endStone->getTexture(face);
    }
    return icon;
}

void TheEndPortalFrameTile::registerIcons(IconRegister* iconRegister) {
    icon = iconRegister->registerIcon("endframe_side");
    iconTop = iconRegister->registerIcon("endframe_top");
    iconEye = iconRegister->registerIcon("endframe_eye");
}

Icon* TheEndPortalFrameTile::getEye() { return iconEye; }

bool TheEndPortalFrameTile::isSolidRender(bool isServerLevel) { return false; }

int TheEndPortalFrameTile::getRenderShape() { return SHAPE_PORTAL_FRAME; }

void TheEndPortalFrameTile::updateDefaultShape() {
    setShape(0, 0, 0, 1, 13.0f / 16.0f, 1);
}

void TheEndPortalFrameTile::addAABBs(Level* level, int x, int y, int z,
                                     AABB* box, std::vector<AABB>* boxes,
                                     std::shared_ptr<Entity> source) {
    setShape(0, 0, 0, 1, 13.0f / 16.0f, 1);
    Tile::addAABBs(level, x, y, z, box, boxes, source);

    int data = level->getData(x, y, z);
    if (hasEye(data)) {
        setShape(5.0f / 16.0f, 13.0f / 16.0f, 5.0f / 16.0f, 11.0f / 16.0f, 1,
                 11.0f / 16.0f);
        Tile::addAABBs(level, x, y, z, box, boxes, source);
    }
    updateDefaultShape();
}

bool TheEndPortalFrameTile::hasEye(int data) { return (data & EYE_BIT) != 0; }

int TheEndPortalFrameTile::getResource(int data, Random* random,
                                       int playerBonusLevel) {
    return 0;
}

void TheEndPortalFrameTile::setPlacedBy(
    Level* level, int x, int y, int z, std::shared_ptr<LivingEntity> by,
    std::shared_ptr<ItemInstance> itemInstance) {
    int dir = (((Mth::floor(by->yRot * 4 / (360) + 0.5)) & 3) + 2) % 4;
    level->setData(x, y, z, dir, Tile::UPDATE_CLIENTS);
}

bool TheEndPortalFrameTile::hasAnalogOutputSignal() { return true; }

int TheEndPortalFrameTile::getAnalogOutputSignal(Level* level, int x, int y,
                                                 int z, int dir) {
    int data = level->getData(x, y, z);

    if (hasEye(data)) {
        return Redstone::SIGNAL_MAX;
    } else {
        return Redstone::SIGNAL_NONE;
    }
}