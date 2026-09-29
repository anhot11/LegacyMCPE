#include "EnchantmentTableTile.h"

#include <memory>

#include "java/Random.h"
#include "minecraft/Facing.h"
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/BaseEntityTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/EnchantmentTableTileEntity.h"

const std::string EnchantmentTableTile::TEXTURE_SIDE = "enchantment_side";
const std::string EnchantmentTableTile::TEXTURE_TOP = "enchantment_top";
const std::string EnchantmentTableTile::TEXTURE_BOTTOM = "enchantment_bottom";

EnchantmentTableTile::EnchantmentTableTile(int id)
    : BaseEntityTile(id, Material::stone, false) {
    updateDefaultShape();
    setLightBlock(0);

    iconTop = nullptr;
    iconBottom = nullptr;
}

// 4J Added override
void EnchantmentTableTile::updateDefaultShape() {
    setShape(0, 0, 0, 1, 12 / 16.0f, 1);
}

bool EnchantmentTableTile::isCubeShaped() { return false; }

void EnchantmentTableTile::animateTick(Level* level, int x, int y, int z,
                                       Random* random) {
    BaseEntityTile::animateTick(level, x, y, z, random);

    for (int xx = x - 2; xx <= x + 2; xx++) {
        for (int zz = z - 2; zz <= z + 2; zz++) {
            if (xx > x - 2 && xx < x + 2 && zz == z - 1) {
                zz = z + 2;
            }
            if (random->nextInt(16) != 0) continue;
            for (int yy = y; yy <= y + 1; yy++) {
                if (level->getTile(xx, yy, zz) == Tile::bookshelf_Id) {
                    if (!level->isEmptyTile((xx - x) / 2 + x, yy,
                                            (zz - z) / 2 + z))
                        break;

                    level->addParticle(eParticleType_enchantmenttable, x + 0.5,
                                       y + 2.0, z + 0.5,
                                       xx - x + random->nextFloat() - 0.5,
                                       yy - y - random->nextFloat() - 1,
                                       zz - z + random->nextFloat() - 0.5);
                }
            }
        }
    }
}

bool EnchantmentTableTile::isSolidRender(bool isServerLevel) { return false; }

Icon* EnchantmentTableTile::getTexture(int face, int data) {
    if (face == Facing::DOWN) return iconBottom;
    if (face == Facing::UP) return iconTop;
    return icon;
}

std::shared_ptr<TileEntity> EnchantmentTableTile::newTileEntity(Level* level) {
    return std::make_shared<EnchantmentTableEntity>();
}

bool EnchantmentTableTile::use(
    Level* level, int x, int y, int z, std::shared_ptr<Player> player,
    int clickedFace, float clickX, float clickY, float clickZ,
    bool soundOnly /*=false*/)  // 4J added soundOnly param
{
    if (soundOnly) return false;

    if (level->isClientSide) {
        return true;
    }
    std::shared_ptr<EnchantmentTableEntity> table =
        std::dynamic_pointer_cast<EnchantmentTableEntity>(
            level->getTileEntity(x, y, z));
    player->startEnchanting(x, y, z,
                            table->hasCustomName() ? table->getName() : "");
    return true;
}

void EnchantmentTableTile::setPlacedBy(
    Level* level, int x, int y, int z, std::shared_ptr<LivingEntity> by,
    std::shared_ptr<ItemInstance> itemInstance) {
    BaseEntityTile::setPlacedBy(level, x, y, z, by, itemInstance);
    if (itemInstance->hasCustomHoverName()) {
        std::dynamic_pointer_cast<EnchantmentTableEntity>(
            level->getTileEntity(x, y, z))
            ->setCustomName(itemInstance->getHoverName());
    }
}

void EnchantmentTableTile::registerIcons(IconRegister* iconRegister) {
    icon = iconRegister->registerIcon(TEXTURE_SIDE);
    iconTop = iconRegister->registerIcon(TEXTURE_TOP);
    iconBottom = iconRegister->registerIcon(TEXTURE_BOTTOM);
}
