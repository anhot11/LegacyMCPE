#include "CommandBlock.h"

#include <memory>
#include <string>

#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/redstone/Redstone.h"
#include "minecraft/world/level/tile/BaseEntityTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/CommandBlockEntity.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"

CommandBlock::CommandBlock(int id)
    : BaseEntityTile(id, Material::metal, false) {}

std::shared_ptr<TileEntity> CommandBlock::newTileEntity(Level* level) {
    return std::make_shared<CommandBlockEntity>();
}

void CommandBlock::neighborChanged(Level* level, int x, int y, int z,
                                   int type) {
    if (!level->isClientSide) {
        bool signal = level->hasNeighborSignal(x, y, z);
        int data = level->getData(x, y, z);
        bool isTriggered = (data & TRIGGER_BIT) != 0;

        if (signal && !isTriggered) {
            level->setData(x, y, z, data | TRIGGER_BIT, Tile::UPDATE_NONE);
            level->addToTickNextTick(x, y, z, id, getTickDelay(level));
        } else if (!signal && isTriggered) {
            level->setData(x, y, z, data & ~TRIGGER_BIT, Tile::UPDATE_NONE);
        }
    }
}

void CommandBlock::tick(Level* level, int x, int y, int z, Random* random) {
    std::shared_ptr<TileEntity> tileEntity = level->getTileEntity(x, y, z);

    if (tileEntity != nullptr &&
        std::dynamic_pointer_cast<CommandBlockEntity>(tileEntity) != nullptr) {
        std::shared_ptr<CommandBlockEntity> commandBlock =
            std::dynamic_pointer_cast<CommandBlockEntity>(tileEntity);
        commandBlock->setSuccessCount(commandBlock->performCommand(level));
        level->updateNeighbourForOutputSignal(x, y, z, id);
    }
}

int CommandBlock::getTickDelay(Level* level) { return 1; }

bool CommandBlock::use(Level* level, int x, int y, int z,
                       std::shared_ptr<Player> player, int clickedFace,
                       float clickX, float clickY, float clickZ,
                       bool soundOnly) {
    std::shared_ptr<CommandBlockEntity> amce =
        std::dynamic_pointer_cast<CommandBlockEntity>(
            level->getTileEntity(x, y, z));

    if (amce != nullptr) {
        player->openTextEdit(amce);
    }

    return true;
}

bool CommandBlock::hasAnalogOutputSignal() { return true; }

int CommandBlock::getAnalogOutputSignal(Level* level, int x, int y, int z,
                                        int dir) {
    std::shared_ptr<TileEntity> tileEntity = level->getTileEntity(x, y, z);

    if (tileEntity != nullptr &&
        std::dynamic_pointer_cast<CommandBlockEntity>(tileEntity) != nullptr) {
        return std::dynamic_pointer_cast<CommandBlockEntity>(tileEntity)
            ->getSuccessCount();
    }

    return Redstone::SIGNAL_NONE;
}

void CommandBlock::setPlacedBy(Level* level, int x, int y, int z,
                               std::shared_ptr<LivingEntity> by,
                               std::shared_ptr<ItemInstance> itemInstance) {
    std::shared_ptr<CommandBlockEntity> cblock =
        std::dynamic_pointer_cast<CommandBlockEntity>(
            level->getTileEntity(x, y, z));

    if (itemInstance->hasCustomHoverName()) {
        cblock->setName(itemInstance->getHoverName());
    }
}

int CommandBlock::getResourceCount(Random* random) { return 0; }