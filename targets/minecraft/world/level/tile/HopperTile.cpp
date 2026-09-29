#include "HopperTile.h"

#include "minecraft/Facing.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/entity/item/ItemEntity.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LevelSource.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/BaseEntityTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/HopperTileEntity.h"
#include "nbt/CompoundTag.h"

class Icon;

const std::string HopperTile::TEXTURE_OUTSIDE = "hopper_outside";
const std::string HopperTile::TEXTURE_INSIDE = "hopper_inside";

HopperTile::HopperTile(int id) : BaseEntityTile(id, Material::metal, false) {
    setShape(0, 0, 0, 1, 1, 1);
}

void HopperTile::updateShape(LevelSource* level, int x, int y, int z,
                             int forceData,
                             std::shared_ptr<TileEntity> forceEntity) {
    setShape(0, 0, 0, 1, 1, 1);
}

void HopperTile::addAABBs(Level* level, int x, int y, int z, AABB* box,
                          std::vector<AABB>* boxes,
                          std::shared_ptr<Entity> source) {
    setShape(0, 0, 0, 1, 10.0f / 16.0f, 1);
    BaseEntityTile::addAABBs(level, x, y, z, box, boxes, source);
    float thickness = 2.0f / 16.0f;
    setShape(0, 0, 0, thickness, 1, 1);
    BaseEntityTile::addAABBs(level, x, y, z, box, boxes, source);
    setShape(0, 0, 0, 1, 1, thickness);
    BaseEntityTile::addAABBs(level, x, y, z, box, boxes, source);
    setShape(1 - thickness, 0, 0, 1, 1, 1);
    BaseEntityTile::addAABBs(level, x, y, z, box, boxes, source);
    setShape(0, 0, 1 - thickness, 1, 1, 1);
    BaseEntityTile::addAABBs(level, x, y, z, box, boxes, source);

    setShape(0, 0, 0, 1, 1, 1);
}

int HopperTile::getPlacedOnFaceDataValue(Level* level, int x, int y, int z,
                                         int face, float clickX, float clickY,
                                         float clickZ, int itemValue) {
    int attached = Facing::OPPOSITE_FACING[face];
    if (attached == Facing::UP) attached = Facing::DOWN;
    return attached;
}

std::shared_ptr<TileEntity> HopperTile::newTileEntity(Level* level) {
    return std::make_shared<HopperTileEntity>();
}

void HopperTile::setPlacedBy(Level* level, int x, int y, int z,
                             std::shared_ptr<LivingEntity> by,
                             std::shared_ptr<ItemInstance> itemInstance) {
    BaseEntityTile::setPlacedBy(level, x, y, z, by, itemInstance);

    if (itemInstance->hasCustomHoverName()) {
        std::shared_ptr<HopperTileEntity> hopper = getHopper(level, x, y, z);
        hopper->setCustomName(itemInstance->getHoverName());
    }
}

void HopperTile::onPlace(Level* level, int x, int y, int z) {
    BaseEntityTile::onPlace(level, x, y, z);
    checkPoweredState(level, x, y, z);
}

bool HopperTile::use(Level* level, int x, int y, int z,
                     std::shared_ptr<Player> player, int clickedFace,
                     float clickX, float clickY, float clickZ, bool soundOnly) {
    if (level->isClientSide) {
        return true;
    }
    std::shared_ptr<HopperTileEntity> hopper = getHopper(level, x, y, z);
    if (hopper != nullptr) player->openHopper(hopper);
    return true;
}

void HopperTile::neighborChanged(Level* level, int x, int y, int z, int type) {
    checkPoweredState(level, x, y, z);
}

void HopperTile::checkPoweredState(Level* level, int x, int y, int z) {
    int data = level->getData(x, y, z);
    int attachedFace = getAttachedFace(data);
    bool shouldBeOn = !level->hasNeighborSignal(x, y, z);
    bool isOn = isTurnedOn(data);

    if (shouldBeOn != isOn) {
        level->setData(x, y, z, attachedFace | (shouldBeOn ? 0 : MASK_TOGGLE),
                       UPDATE_NONE);
    }
}

void HopperTile::onRemove(Level* level, int x, int y, int z, int id, int data) {
    std::shared_ptr<Container> container =
        std::dynamic_pointer_cast<HopperTileEntity>(
            level->getTileEntity(x, y, z));
    if (container != nullptr) {
        for (int i = 0; i < container->getContainerSize(); i++) {
            std::shared_ptr<ItemInstance> item = container->getItem(i);
            if (item != nullptr) {
                float xo = random.nextFloat() * 0.8f + 0.1f;
                float yo = random.nextFloat() * 0.8f + 0.1f;
                float zo = random.nextFloat() * 0.8f + 0.1f;

                while (item->count > 0) {
                    int count = random.nextInt(21) + 10;
                    if (count > item->count) count = item->count;
                    item->count -= count;

                    std::shared_ptr<ItemEntity> itemEntity =
                        std::make_shared<ItemEntity>(
                            level, x + xo, y + yo, z + zo,
                            std::make_shared<ItemInstance>(
                                item->id, count, item->getAuxValue()));

                    if (item->hasTag()) {
                        itemEntity->getItem()->setTag(
                            (CompoundTag*)item->getTag()->copy());
                    }

                    float pow = 0.05f;
                    itemEntity->xd = (float)random.nextGaussian() * pow;
                    itemEntity->yd = (float)random.nextGaussian() * pow + 0.2f;
                    itemEntity->zd = (float)random.nextGaussian() * pow;
                    level->addEntity(itemEntity);
                }
            }
        }
        level->updateNeighbourForOutputSignal(x, y, z, id);
    }

    BaseEntityTile::onRemove(level, x, y, z, id, data);
}

int HopperTile::getRenderShape() { return SHAPE_HOPPER; }

bool HopperTile::isCubeShaped() { return false; }

bool HopperTile::isSolidRender(bool isServerLevel /*= false*/) { return false; }

bool HopperTile::shouldRenderFace(LevelSource* level, int x, int y, int z,
                                  int face) {
    return true;
}

Icon* HopperTile::getTexture(int face, int data) {
    if (face == Facing::UP) {
        return hopperTopIcon;
    }
    return hopperIcon;
}

int HopperTile::getAttachedFace(int data) { return data & MASK_ATTACHED; }

bool HopperTile::isTurnedOn(int data) {
    return (data & MASK_TOGGLE) != MASK_TOGGLE;
}

bool HopperTile::hasAnalogOutputSignal() { return true; }

int HopperTile::getAnalogOutputSignal(Level* level, int x, int y, int z,
                                      int dir) {
    return AbstractContainerMenu::getRedstoneSignalFromContainer(
        getHopper(level, x, y, z));
}

void HopperTile::registerIcons(IconRegister* iconRegister) {
    hopperIcon = iconRegister->registerIcon(TEXTURE_OUTSIDE);
    hopperTopIcon = iconRegister->registerIcon("hopper_top");
    hopperInnerIcon = iconRegister->registerIcon(TEXTURE_INSIDE);
}

Icon* HopperTile::getTexture(const std::string& name) {
    if (name.compare(TEXTURE_OUTSIDE) == 0) return Tile::hopper->hopperIcon;
    if (name.compare(TEXTURE_INSIDE) == 0) return Tile::hopper->hopperInnerIcon;
    return nullptr;
}

std::string HopperTile::getTileItemIconName() { return "hopper"; }

std::shared_ptr<HopperTileEntity> HopperTile::getHopper(LevelSource* level,
                                                        int x, int y, int z) {
    return std::dynamic_pointer_cast<HopperTileEntity>(
        level->getTileEntity(x, y, z));
}