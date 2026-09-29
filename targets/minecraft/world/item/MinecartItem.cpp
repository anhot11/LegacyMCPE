#include "MinecartItem.h"

#include <memory>
#include <string>

#include "java/Class.h"
#include "minecraft/core/BehaviorRegistry.h"
#include "minecraft/core/BlockSource.h"
#include "minecraft/core/DefaultDispenseItemBehavior.h"
#include "minecraft/core/FacingEnum.h"
#include "minecraft/world/entity/item/Minecart.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/BaseRailTile.h"
#include "minecraft/world/level/tile/DispenserTile.h"
#include "minecraft/world/level/tile/LevelEvent.h"

std::shared_ptr<ItemInstance> MinecartItem::MinecartDispenseBehavior::execute(
    BlockSource* source, std::shared_ptr<ItemInstance> dispensed,
    eOUTCOME& outcome) {
    FacingEnum* facing = DispenserTile::getFacing(source->getData());
    Level* world = source->getWorld();

    // Spawn the minecart 'just' outside the dispenser, it overlaps 2 'pixels'
    // now. Also at half-block-height so it can connect with sloped rails
    double spawnX = source->getX() + facing->getStepX() * (1 + 2.0f / 16);
    double spawnY = source->getY() + facing->getStepY() * (1 + 2.0f / 16);
    double spawnZ = source->getZ() + facing->getStepZ() * (1 + 2.0f / 16);

    int frontX = source->getBlockX() + facing->getStepX();
    int frontY = source->getBlockY() + facing->getStepY();
    int frontZ = source->getBlockZ() + facing->getStepZ();
    int inFront = world->getTile(frontX, frontY, frontZ);

    // 4J: If we're at limit, just dispense item (instead of adding minecart)
    if (world->countInstanceOf(eTYPE_MINECART, false) >=
        Level::MAX_CONSOLE_MINECARTS) {
        outcome = DISPENCED_ITEM;
        return defaultDispenseItemBehavior.dispense(source, dispensed);
    }

    double yOffset;
    if (BaseRailTile::isRail(inFront)) {
        yOffset = 0;
    } else if (inFront == 0 && BaseRailTile::isRail(world->getTile(
                                   frontX, frontY - 1, frontZ))) {
        yOffset = -1;
    } else {
        outcome = DISPENCED_ITEM;
        return defaultDispenseItemBehavior.dispense(source, dispensed);
    }

    outcome = ACTIVATED_ITEM;

    std::shared_ptr<Minecart> minecart =
        Minecart::createMinecart(world, spawnX, spawnY + yOffset, spawnZ,
                                 ((MinecartItem*)dispensed->getItem())->type);
    if (dispensed->hasCustomHoverName()) {
        minecart->setCustomName(dispensed->getHoverName());
    }
    world->addEntity(minecart);

    dispensed->remove(1);
    return dispensed;
}

void MinecartItem::MinecartDispenseBehavior::playSound(BlockSource* source) {
    source->getWorld()->levelEvent(LevelEvent::SOUND_CLICK, source->getBlockX(),
                                   source->getBlockY(), source->getBlockZ(), 0);
}

MinecartItem::MinecartItem(int id, int type) : Item(id) {
    maxStackSize = 1;
    this->type = type;
    DispenserTile::REGISTRY.add(this, new MinecartDispenseBehavior());
}

bool MinecartItem::useOn(std::shared_ptr<ItemInstance> instance,
                         std::shared_ptr<Player> player, Level* level, int x,
                         int y, int z, int face, float clickX, float clickY,
                         float clickZ, bool bTestUseOnOnly) {
    // 4J-PB - Adding a test only version to allow tooltips to be displayed
    int targetType = level->getTile(x, y, z);

    if (BaseRailTile::isRail(targetType)) {
        if (!bTestUseOnOnly) {
            if (!level->isClientSide) {
                std::shared_ptr<Minecart> cart = Minecart::createMinecart(
                    level, x + 0.5f, y + 0.5f, z + 0.5f, type);
                if (instance->hasCustomHoverName()) {
                    cart->setCustomName(instance->getHoverName());
                }
                level->addEntity(cart);
            }
            instance->count--;
        }
        return true;
    }
    return false;
}