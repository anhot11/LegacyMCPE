#include "AnvilTile.h"

#include "Direction.h"
#include "Facing.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/item/FallingTile.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LevelSource.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/HeavyTile.h"
#include "minecraft/world/level/tile/LevelEvent.h"
#include "minecraft/world/level/tile/Tile.h"
#include "strings.h"

class Icon;

const unsigned int AnvilTile::ANVIL_NAMES[ANVIL_NAMES_LENGTH] = {
    IDS_TILE_ANVIL_INTACT,
    IDS_TILE_ANVIL_SLIGHTLYDAMAGED,
    IDS_TILE_ANVIL_VERYDAMAGED,
};

std::string AnvilTile::TEXTURE_DAMAGE_NAMES[ANVIL_NAMES_LENGTH] = {
    "anvil_top", "anvil_top_damaged_1", "anvil_top_damaged_2"};

AnvilTile::AnvilTile(int id) : HeavyTile(id, Material::heavyMetal, false) {
    part = PART_BASE;
    setLightBlock(0);
    icons = nullptr;
}

bool AnvilTile::isCubeShaped() { return false; }

bool AnvilTile::isSolidRender(bool isServerLevel) { return false; }

Icon* AnvilTile::getTexture(int face, int data) {
    if (part == PART_TOP && face == Facing::UP) {
        int damage = (data >> 2) % ANVIL_NAMES_LENGTH;
        return icons[damage];
    }
    return icon;
}

void AnvilTile::registerIcons(IconRegister* iconRegister) {
    icon = iconRegister->registerIcon("anvil_base");
    icons = new Icon*[ANVIL_NAMES_LENGTH];

    for (int i = 0; i < ANVIL_NAMES_LENGTH; i++) {
        icons[i] = iconRegister->registerIcon(TEXTURE_DAMAGE_NAMES[i]);
    }
}

void AnvilTile::setPlacedBy(Level* level, int x, int y, int z,
                            std::shared_ptr<LivingEntity> by,
                            std::shared_ptr<ItemInstance> itemInstance) {
    int dir = (Mth::floor(by->yRot * 4 / (360) + 0.5)) & 3;
    int dmg = level->getData(x, y, z) >> 2;

    dir = ++dir % 4;
    if (dir == 0)
        level->setData(x, y, z, Direction::NORTH | (dmg << 2),
                       Tile::UPDATE_CLIENTS);
    if (dir == 1)
        level->setData(x, y, z, Direction::EAST | (dmg << 2),
                       Tile::UPDATE_CLIENTS);
    if (dir == 2)
        level->setData(x, y, z, Direction::SOUTH | (dmg << 2),
                       Tile::UPDATE_CLIENTS);
    if (dir == 3)
        level->setData(x, y, z, Direction::WEST | (dmg << 2),
                       Tile::UPDATE_CLIENTS);
}

bool AnvilTile::use(Level* level, int x, int y, int z,
                    std::shared_ptr<Player> player, int clickedFace,
                    float clickX, float clickY, float clickZ, bool soundOnly) {
    if (level->isClientSide) {
        return true;
    }
    player->startRepairing(x, y, z);
    return true;
}

int AnvilTile::getRenderShape() { return SHAPE_ANVIL; }

int AnvilTile::getSpawnResourcesAuxValue(int data) { return data >> 2; }

void AnvilTile::updateShape(LevelSource* level, int x, int y, int z,
                            int forceData,
                            std::shared_ptr<TileEntity> forceEntity) {
    int dir = level->getData(x, y, z) & 3;

    if (dir == Direction::EAST || dir == Direction::WEST) {
        setShape(0, 0, 2 / 16.0f, 1, 1, 1 - 2 / 16.0f);
    } else {
        setShape(2 / 16.0f, 0, 0, 1 - 2 / 16.0f, 1, 1);
    }
}

void AnvilTile::falling(std::shared_ptr<FallingTile> entity) {
    entity->setHurtsEntities(true);
}

void AnvilTile::onLand(Level* level, int xt, int yt, int zt, int data) {
    level->levelEvent(LevelEvent::SOUND_ANVIL_LAND, xt, yt, zt, 0);
}

bool AnvilTile::shouldRenderFace(LevelSource* level, int x, int y, int z,
                                 int face) {
    return true;
}