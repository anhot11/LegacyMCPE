#include "BrewingStandTile.h"

#include <memory>
#include <string>

#include "java/Random.h"
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/entity/item/ItemEntity.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/BaseEntityTile.h"
#include "minecraft/world/level/tile/entity/BrewingStandTileEntity.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "nbt/CompoundTag.h"

class Icon;

BrewingStandTile::BrewingStandTile(int id)
    : BaseEntityTile(id, Material::metal, false) {
    random = new Random();
    iconBase = nullptr;
}

BrewingStandTile::~BrewingStandTile() { delete random; }

bool BrewingStandTile::isSolidRender(bool isServerLevel) { return false; }

int BrewingStandTile::getRenderShape() { return SHAPE_BREWING_STAND; }

std::shared_ptr<TileEntity> BrewingStandTile::newTileEntity(Level* level) {
    return std::make_shared<BrewingStandTileEntity>();
}

bool BrewingStandTile::isCubeShaped() { return false; }

void BrewingStandTile::addAABBs(Level* level, int x, int y, int z, AABB* box,
                                std::vector<AABB>* boxes,
                                std::shared_ptr<Entity> source) {
    setShape(7.0f / 16.0f, 0, 7.0f / 16.0f, 9.0f / 16.0f, 14.0f / 16.0f,
             9.0f / 16.0f);
    BaseEntityTile::addAABBs(level, x, y, z, box, boxes, source);
    updateDefaultShape();
    BaseEntityTile::addAABBs(level, x, y, z, box, boxes, source);
}

void BrewingStandTile::updateDefaultShape() {
    setShape(0, 0, 0, 1, 2.0f / 16.0f, 1);
}

bool BrewingStandTile::use(
    Level* level, int x, int y, int z, std::shared_ptr<Player> player,
    int clickedFace, float clickX, float clickY, float clickZ,
    bool soundOnly /*=false*/)  // 4J added soundOnly param
{
    if (soundOnly) return false;

    if (level->isClientSide) {
        return true;
    }
    std::shared_ptr<BrewingStandTileEntity> brewingStand =
        std::dynamic_pointer_cast<BrewingStandTileEntity>(
            level->getTileEntity(x, y, z));
    if (brewingStand != nullptr) player->openBrewingStand(brewingStand);

    return true;
}

void BrewingStandTile::setPlacedBy(Level* level, int x, int y, int z,
                                   std::shared_ptr<LivingEntity> by,
                                   std::shared_ptr<ItemInstance> itemInstance) {
    if (itemInstance->hasCustomHoverName()) {
        std::dynamic_pointer_cast<BrewingStandTileEntity>(
            level->getTileEntity(x, y, z))
            ->setCustomName(itemInstance->getHoverName());
    }
}

void BrewingStandTile::animateTick(Level* level, int xt, int yt, int zt,
                                   Random* random) {
    double x = xt + 0.4f + random->nextFloat() * 0.2f;
    double y = yt + 0.7f + random->nextFloat() * 0.3f;
    double z = zt + 0.4f + random->nextFloat() * 0.2f;

    level->addParticle(eParticleType_smoke, x, y, z, 0, 0, 0);
}

void BrewingStandTile::onRemove(Level* level, int x, int y, int z, int id,
                                int data) {
    std::shared_ptr<TileEntity> tileEntity = level->getTileEntity(x, y, z);
    if (tileEntity != nullptr &&
        (std::dynamic_pointer_cast<BrewingStandTileEntity>(tileEntity) !=
         nullptr)) {
        std::shared_ptr<BrewingStandTileEntity> container =
            std::dynamic_pointer_cast<BrewingStandTileEntity>(tileEntity);
        for (int i = 0; i < container->getContainerSize(); i++) {
            std::shared_ptr<ItemInstance> item = container->getItem(i);
            if (item != nullptr) {
                float xo = random->nextFloat() * 0.8f + 0.1f;
                float yo = random->nextFloat() * 0.8f + 0.1f;
                float zo = random->nextFloat() * 0.8f + 0.1f;

                while (item->count > 0) {
                    int count = random->nextInt(21) + 10;
                    if (count > item->count) count = item->count;
                    item->count -= count;

                    std::shared_ptr<ItemEntity> itemEntity =
                        std::make_shared<ItemEntity>(
                            level, x + xo, y + yo, z + zo,
                            std::make_shared<ItemInstance>(
                                item->id, count, item->getAuxValue()));
                    float pow = 0.05f;
                    itemEntity->xd = (float)random->nextGaussian() * pow;
                    itemEntity->yd = (float)random->nextGaussian() * pow + 0.2f;
                    itemEntity->zd = (float)random->nextGaussian() * pow;
                    if (item->hasTag()) {
                        itemEntity->getItem()->setTag(
                            (CompoundTag*)item->getTag()->copy());
                    }
                    level->addEntity(itemEntity);
                }
            }
        }
    }
    BaseEntityTile::onRemove(level, x, y, z, id, data);
}

int BrewingStandTile::getResource(int data, Random* random,
                                  int playerBonusLevel) {
    return Item::brewingStand_Id;
}

int BrewingStandTile::cloneTileId(Level* level, int x, int y, int z) {
    return Item::brewingStand_Id;
}

bool BrewingStandTile::hasAnalogOutputSignal() { return true; }

int BrewingStandTile::getAnalogOutputSignal(Level* level, int x, int y, int z,
                                            int dir) {
    return AbstractContainerMenu::getRedstoneSignalFromContainer(
        std::dynamic_pointer_cast<Container>(level->getTileEntity(x, y, z)));
}

void BrewingStandTile::registerIcons(IconRegister* iconRegister) {
    BaseEntityTile::registerIcons(iconRegister);
    iconBase = iconRegister->registerIcon(getIconName() + "_base");
}

Icon* BrewingStandTile::getBaseTexture() { return iconBase; }